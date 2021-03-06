
// upgradeDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "upgrade.h"
#include "upgradeDlg.h"
#include "afxdialogex.h"
#include "Setting.h"
#include "cequ.h"
#include "global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CupgradeDlg 对话框

#define TIMER_WAIT_ACK 2000
#define TIMER_BYTE_ACK 2001

CSerialPort m_Comm;
CString m_filename;
CWinThread *pThreadUpgrade;
bool isThreadPause;
bool m_isAckOk;
bool m_downloading;
CEvent m_Event;
HANDLE m_handle;

CStatusBarCtrl *m_StatBar;
CStatusBar m_StatusBar;


static UINT BASED_CODE indicators[] =
//状态栏的指示器列表，如有未定义的字符串名，需在Resource View的String Table中添加定义
{
ID_INDICATOR_COM,
ID_INDICATOR_TYPE,
ID_INDICATOR_FILE,
ID_INDICATOR_PWD,//需在Resource View的String Table中添加定义
};



CupgradeDlg::CupgradeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_UPGRADE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CupgradeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
}

BEGIN_MESSAGE_MAP(CupgradeDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	
	ON_COMMAND(ID_MENU_SETTING, &CupgradeDlg::OnMenuSetting)
	ON_BN_CLICKED(IDCANCEL, &CupgradeDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CupgradeDlg::OnBnClickedOk)
	ON_COMMAND(ID_MENU_FILE, &CupgradeDlg::OnMenuFile)
	ON_WM_TIMER()
	ON_MESSAGE(WM_COMM_RXCHAR, &CupgradeDlg::OnCommRxchar)
	ON_MESSAGE(WM_COMM_RXSTR, &CupgradeDlg::OnCommRxstr)
	ON_COMMAND(ID_MENU_START, &CupgradeDlg::OnMenuStart)
	ON_COMMAND(ID_MENU_EQU, &CupgradeDlg::OnMenuEqu)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CupgradeDlg 消息处理程序

BOOL CupgradeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	m_progress.ShowWindow(false);
	CStatic *ctext = (CStatic*)GetDlgItem(IDC_STATIC);
	ctext->ShowWindow(false);
	com_set.port = 3;
	com_set.baud = 9600;
	CMenu menu;
	menu.LoadMenuW(IDR_MENU);
	SetMenu(&menu);

	equ.type = 0;
	equ.addrh = 0;
	equ.addrl = 0x10;
	//pThreadUpgrade = AfxBeginThread(ThreadTimer, this, THREAD_PRIORITY_NORMAL, 0, 0);

#if 0
	m_StatBar = new CStatusBarCtrl;
	RECT m_Rect;
	GetClientRect(&m_Rect); //获取对话框的矩形区域
	m_Rect.top = m_Rect.bottom - 20; //设置状态栏的矩形区域
	m_StatBar->Create(WS_BORDER | WS_VISIBLE | CBRS_BOTTOM, m_Rect, this, 4);

	int nParts[4] = { 100, 300, 200,-1 }; //分割尺寸
	m_StatBar->SetParts(4, nParts); //分割状态栏
	m_StatBar->SetText(_T("串口:"), 0, 0); //第一个分栏加入"这是第一个指示器"
	m_StatBar->SetText(_T("文件:"), 1, 0); //以下类似
	m_StatBar->SetText(_T("校验码:"), 2, 0); //以下类似
	/*也可使用以下方式加入指示器文字
	m_StatBar.SetPaneText(0,"这是第一个指示器",0);*/
#endif
	m_StatusBar.CreateEx(this, SBT_TOOLTIPS, WS_CHILD | WS_VISIBLE | CBRS_BOTTOM, AFX_IDW_STATUS_BAR);
	m_StatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));//设置指示器数量
	CRect rect;
	GetClientRect(&rect);

	m_StatusBar.SetPaneInfo(0, ID_INDICATOR_COM, SBPS_NORMAL, rect.Width() / 4-50);//设置指示器窗口的有关信息
	m_StatusBar.SetPaneInfo(1, ID_INDICATOR_FILE, SBPS_STRETCH, rect.Width() / 4-30);
	m_StatusBar.SetPaneInfo(2, ID_INDICATOR_FILE, SBPS_STRETCH, rect.Width() / 4+60);
	m_StatusBar.SetPaneInfo(3, ID_INDICATOR_PWD, SBPS_STRETCH, rect.Width() / 4+20);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST, ID_INDICATOR_COM);//个语句的作用就是让这个状态栏根据窗口大小自动调整最终显示在对话框中（在文档程序中不必要）
	m_StatusBar.GetStatusBarCtrl().SetBkColor(RGB(180, 20, 180));//设置背景

	CString strCom;
	CString strEqu;

	strCom.Format(_T("COM%d"), com_set.port);
	m_StatusBar.SetPaneText(0, _T("串口: ")+strCom, 0);
	if (equ.type == 0)
		strEqu = "梯控小板";
	else
		strEqu = "梯控大板";
	m_StatusBar.SetPaneText(1, _T("设备: ") + strEqu, 0);
	m_StatusBar.SetPaneText(2, _T("文件: "), 0);
	m_StatusBar.SetPaneText(3, _T("校验码: "), 0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CupgradeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CupgradeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}







void CupgradeDlg::OnMenuSetting()
{
	// TODO: 在此添加命令处理程序代码
	Setting setDlg;
	setDlg.DoModal();
	
}


void CupgradeDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码


	isThreadPause = true;

	//CDialogEx::OnCancel();
}



int CupgradeDlg::ReadFileData(unsigned char *rbuf, CString filename)
{
	return 0;
}

#if 0
LRESULT CupgradeDlg::OnReceiveStr(WPARAM str, LPARAM commInfo)
{
	struct serialPortInfo
	{
		UINT portNr;//串口号
		DWORD bytesRead;//读取的字节数
	}*pCommInfo;
	pCommInfo = (serialPortInfo*)commInfo;

	CString str1((char*)str);
	//char* m_str = (char*)str;
	//TCHAR* dest = NULL;
	//dest = new TCHAR[sizeof(TCHAR) * pCommInfo->bytesRead];
	//wmemcpy(dest, (TCHAR*)str, sizeof(TCHAR)* pCommInfo->bytesRead+1);

	//获取指定串口的数据
	//if (pCommInfo->portNr == 2)
	//{
	
	//}
	return TRUE;
}
#endif


/*
思路：
定义两个定时器，分别为接收超时定时器1和字节超时定时器2
T1：发送后，启动T1（1000MS），如果超过这个时间没有回复，认为通讯失败，关闭T1，弹出提示
T2:发送后，在T1时间内，收到了回复，这个时候数据可能还没收全，需要关闭T1，开启T2（100MS），只要T2内有接收到
数据，超时重新赋值，直到超时时间到，进行数据处理
*/
int  CupgradeDlg::WaitComAck(int nTimer)
{
	m_isAckOk = false;
	com_ack.start = true;
	com_ack.cnt = nTimer / 100;
	while (com_ack.start && !com_byte.start)
	{
		XSleep(100);
		if (!(--com_ack.cnt))
		{
			com_ack.start = false;
		}
	}
	while (com_byte.start)
	{
		XSleep(10);
		if (!(--com_byte.cnt))
		{
			com_byte.start = 0;
			CheckRecBuf();
		}
	}

	
	//SetTimer(TIMER_WAIT_ACK, 100, NULL);
	//XSleep(nTimer);
	return 0;
}

UINT __cdecl CupgradeDlg::ThreadTimer(LPVOID pParam)
{
	CupgradeDlg *threadol = (CupgradeDlg*)pParam;
	//将this指针传递给threadol对象。通过 "threadol->" 的方式，使得线程1中可以调用主线程的所有函数，全局变量，控件
	
	return 0;
}
UINT __cdecl CupgradeDlg::ThreadUpgrade(LPVOID pParam)
{
	//CthreadDlg *threadol = new CthreadDlg; 
	//CthreadDlg *threadol=new CthreadDlg(); 
	CupgradeDlg *threadol = (CupgradeDlg*)pParam;
	//将this指针传递给threadol对象。通过 "threadol->" 的方式，使得线程1中可以调用主线程的所有函数，全局变量，控件
	
	unsigned char fcs;
	unsigned char sbuf[180];
	unsigned int package;
	unsigned int packsum;
	CString strPer;

	sbuf[0] = HEAD;
	sbuf[1] = 0x8b;
	sbuf[2] = 0x11;
	sbuf[3] = 0x00;
	sbuf[4] = 0x00;
	sbuf[5] = 0x00;
	sbuf[6] = 0x00;

	sbuf[7] = 0x00;
	sbuf[8] = 0x00;
	if (equ.type == 0)
	{
		sbuf[9] = 0x00;
		sbuf[10] = 0x10;
	}
	else
	{
		sbuf[9] = 0x03;
		sbuf[10] = 0x01;
	}


	m_downloading = true;
	package = 0;
	threadol->m_progress.SetRange(0, (short)up_file.len);
	threadol->m_progress.SetPos(0);
	//packsum = up_file.len / 128;
	while (!isThreadPause)
	{
		sbuf[11] = LOBYTE(package);
		sbuf[12] = HIBYTE(package);
		memcpy(sbuf + 13, up_file.buf + package*128, 128);
		fcs = 0;
		for (int i = 0; i < 141; i++)
			fcs += sbuf[i];
		sbuf[141] = fcs;


		m_Comm.WriteToPort(sbuf, 142);
		threadol->WaitComAck(ACKWAIT);

		if (!m_isAckOk)
		{
			isThreadPause = true;
			AfxMessageBox(_T("通讯失败"));
		}
		package++;
		threadol->m_progress.SetStep(1);
		threadol->m_progress.StepIt();
		if (package >= up_file.len)
			isThreadPause = true;
		float a, b;
		float fper = 0.00;
		a = (float)package;
		b = (float)up_file.len;
		fper = a * 100 / b;
		strPer.Format(_T("%.2f%%"), fper);
		threadol->GetDlgItem(IDC_STATIC)->SetWindowText(strPer);
	}
	m_downloading = false;
	return 0;
}




void CupgradeDlg::OnSendMessage(unsigned char* str, int port, int str_len)
{
#if 0
	com_ack.start = false;
	KillTimer(TIMER_WAIT_ACK);
	memcpy(com_rec.buf + com_rec.p, str, str_len);
	com_rec.p += str_len;
	com_byte.start = true;
	com_byte.cnt = BYTEWAIT / 20;
	SetTimer(TIMER_BYTE_ACK, 20, NULL);
#else
	//SetEvent(m_handle);       //触发事件
	m_Event.SetEvent();
#endif

	

}


struct XSleep_Structure
{
	int duration;
	HANDLE evenHandle;
};


DWORD WINAPI XSleepThread(LPVOID pWaitTime)
{
	XSleep_Structure *sleep = (XSleep_Structure*)pWaitTime;


	Sleep(sleep->duration);
	SetEvent(sleep->evenHandle);


	return 0;
}


// 非阻塞延时
void CupgradeDlg::XSleep(int nWaitInMsecs)
{
	XSleep_Structure sleep;
	sleep.duration = nWaitInMsecs;
	sleep.evenHandle = CreateEvent(NULL, TRUE, FALSE, NULL);

	DWORD dwThreadId;
	HANDLE hThread=CreateThread(NULL, 0, &XSleepThread, &sleep, 0, &dwThreadId);


	MSG msg;
	while (::WaitForSingleObject(sleep.evenHandle, 0) == WAIT_TIMEOUT)// && !m_isAckOk)
	{
		// get and dispatch message
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
	CloseHandle(sleep.evenHandle);
	//CloseHandle(hThread);
}





void CupgradeDlg::OnBnClickedOk()
{
	
	
}

CString GetModuleDir()
{
	CString path;
	GetModuleFileName(NULL, path.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	path.ReleaseBuffer();
	int pos = path.ReverseFind('\\');
	path = path.Left(pos);
	return path;
}


void CupgradeDlg::OnMenuFile()
{
	// TODO: 在此添加命令处理程序代码
	CString filter;
	CString path;
	CString strFile;

	path = GetModuleDir();
	filter = "bin文件(*.bin)|*.bin|hex文件(*.hex)|*.hex|所有文件(*.*)|*.*||";
	CFileDialog dlg(TRUE, NULL, path, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);
	if (dlg.DoModal() == IDOK)
	{
		CString filepathname;
		filepathname = dlg.GetPathName();
		strFile = dlg.GetFileName();
		m_filename = filepathname;
	}
	else
		m_filename = "";

	
	

	m_StatusBar.SetPaneText(2, _T("文件: ") + strFile, 0);
	m_StatusBar.Invalidate();

	FILE *pFile;
	_wfopen_s(&pFile, m_filename, L"rb"); //打开文件strFilePath是文件路径VS2010是UNICODE编码 定义时注意转换或者这样L"xxx"
	if (pFile == NULL) //判断文件是否打开成功
	{
		MessageBox(L"文件打开失败", L"warning", MB_ICONSTOP);
		return;
	}

	fseek(pFile, 0, SEEK_END);//将文件指针设置到文件末尾处
	up_file.len = ftell(pFile);//获取文件指针的位置 也就相当于文件的大小了
	fseek(pFile, 0, SEEK_SET);//重新将文件指针调回文件开头
	fread(up_file.buf, sizeof(unsigned char), up_file.len, pFile);//将整个文件读取 注意这里文件的大小不应超过65536
	fclose(pFile);//关闭文件

	CString strPWD;
	char cPWD[9];
	memcpy(cPWD, up_file.buf + up_file.len - 8, 8);
	cPWD[8] = 0;
	strPWD = cPWD;
	m_StatusBar.SetPaneText(3, _T("校验码: ") + strPWD, 0);
	m_StatusBar.Invalidate();


}

int CupgradeDlg::CheckRecBuf(void)
{
	int i;
	int j;
	int len;
	bool isAakOk = false;
	unsigned char fcs;

	for (i = 0; i < com_rec.p; i++)
	{
		if (com_rec.buf[i] == HEAD) //起始位正确
		{
			if (com_rec.buf[i+1]==6)// < COMM_LEN_MAX ) //长度在范围内
			{
				len = com_rec.buf[i + 1];
				fcs = 0;
				for (j = i; j < i + len + 2; j++)
					fcs += com_rec.buf[j];
				if (fcs == com_rec.buf[i + len + 2]) //校验和正确
				{
					isAakOk=true;
					break;
				}
			}
		}
	}
	if (isAakOk)
	{
		if (com_rec.buf[i+1] == 0x06 && com_rec.buf[i+2] == 0x12 && com_rec.buf[i+3] == 0x00)
		{
			//pThreadUpgrade = AfxBeginThread(ThreadUpgrade, this);
			//pThreadUpgrade = AfxBeginThread(ThreadUpgrade, this, THREAD_PRIORITY_NORMAL, 0, 0);
			m_isAckOk = true;
			//SetEvent(m_handle);       //触发事件
		}
	}
	memset(&com_rec, 0, sizeof(com_rec));
	return 0;
}

void CupgradeDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == TIMER_BYTE_ACK)
	{
		if (com_byte.start)
		{
			if (!(--com_byte.cnt))
			{
				com_byte.start = 0;
				KillTimer(TIMER_BYTE_ACK);
				CheckRecBuf();
			}
		}
		
	}
	else if (nIDEvent == TIMER_WAIT_ACK)
	{
		if (com_ack.start)
		{
			if (!(--com_ack.cnt))
			{
				com_ack.start = 0;
				KillTimer(TIMER_WAIT_ACK);
				SetEvent(m_handle);       //触发事件
				
			}
		}
		//KillTimer(TIMER_WAIT_ACK);
		//AfxMessageBox(_T("通讯失败"));
	}
	__super::OnTimer(nIDEvent);
}


afx_msg LRESULT CupgradeDlg::OnCommRxchar(WPARAM wParam, LPARAM lParam)
{
	
	return 0;
}


afx_msg LRESULT CupgradeDlg::OnCommRxstr(WPARAM wParam, LPARAM lParam)
{
	struct serialPortInfo
	{
		UINT portNr;//串口号
		DWORD bytesRead;//读取的字节数
	}*pCommInfo;
	pCommInfo = (serialPortInfo*)lParam;

	com_ack.start = false;
	//KillTimer(TIMER_WAIT_ACK);
	memcpy(com_rec.buf + com_rec.p, (unsigned char*)wParam, pCommInfo->bytesRead);
	com_rec.p += pCommInfo->bytesRead;
	com_byte.start = true;
	com_byte.cnt = BYTEWAIT / 20;
	//SetTimer(TIMER_BYTE_ACK, 20, NULL);


	return 0;
}


void CupgradeDlg::OnMenuStart()
{
	// TODO: 在此添加命令处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	int cnt;

	if (m_downloading)
	{
		AfxMessageBox(_T("正在升级中..."));
		return;
	}
	if (m_Comm.InitPort(this->GetSafeHwnd(), com_set.port, com_set.baud))
	{

		m_Comm.StartMonitoring();

	}
	else
	{
		AfxMessageBox(_T("串口已被占用！"));
		return;
	}
	//读取文件
	//unsigned long nFileBytes;//用于存储BIN文件总字节数
	//unsigned char aBinByte[65536];//用于储存从BIN文件获取的数据
	FILE *pFile;
	_wfopen_s(&pFile, m_filename, L"rb"); //打开文件strFilePath是文件路径VS2010是UNICODE编码 定义时注意转换或者这样L"xxx"
	if (pFile == NULL) //判断文件是否打开成功
	{
		MessageBox(L"文件打开失败", L"warning", MB_ICONSTOP);
		return;
	}

	fseek(pFile, 0, SEEK_END);//将文件指针设置到文件末尾处
	up_file.len = ftell(pFile);//获取文件指针的位置 也就相当于文件的大小了
	fseek(pFile, 0, SEEK_SET);//重新将文件指针调回文件开头
	fread(up_file.buf, sizeof(unsigned char), up_file.len, pFile);//将整个文件读取 注意这里文件的大小不应超过65536
	fclose(pFile);//关闭文件

	//START UPDATA
	unsigned char fcs;
	unsigned char sbuf[180];
	sbuf[0] = HEAD;
	sbuf[1] = 0x13;
	sbuf[2] = 0x10;
	sbuf[3] = 0x00;
	sbuf[4] = 0x00;
	sbuf[5] = 0x00;
	sbuf[6] = 0x00;

	sbuf[7] = 0x00;
	sbuf[8] = 0x00;
	if (equ.type == 0)
	{
		sbuf[9] = 0x00;
		sbuf[10] = 0x10;
	}
	else
	{
		sbuf[9] = 0x03;
		sbuf[10] = 0x01;
	}
	
	
	memcpy(sbuf + 13, up_file.buf + up_file.len - 8, 8);
	up_file.len /= 128;
	sbuf[11] = LOBYTE(up_file.len);
	sbuf[12] = HIBYTE(up_file.len);
	fcs = 0;
	for (int i = 0; i < 21; i++)
		fcs += sbuf[i];
	sbuf[21] = fcs;
	//START UPGRADE

	m_Comm.WriteToPort(sbuf, 22);
#ifdef _SEND_DATA_WITH_SIGSLOT
	m_Comm.sendMessageSignal.connect(this, &CupgradeDlg::OnSendMessage);
#endif
	memset(&com_ack, 0, sizeof(com_ack));
	memset(&com_byte, 0, sizeof(com_byte));
	WaitComAck(ACKWAIT);

	//WaitForSingleObject(m_Event, 3000);
	
	if (m_isAckOk)
	{
		isThreadPause = false;
		m_progress.ShowWindow(true);
		CStatic *ctext = (CStatic*)GetDlgItem(IDC_STATIC);
		ctext->ShowWindow(true);
		pThreadUpgrade = AfxBeginThread(ThreadUpgrade, this, THREAD_PRIORITY_NORMAL, 0, 0);
		//pThreadUpgrade = AfxBeginThread(ThreadUpgrade, this);
	}
	else
		AfxMessageBox(_T("通讯失败"));
}


void CupgradeDlg::OnMenuEqu()
{
	// TODO: 在此添加命令处理程序代码
	CEqu equDlg;
	equDlg.DoModal();
}


void CupgradeDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_Comm.ClosePort();
	__super::OnClose();
	CDialogEx::OnOK();
}

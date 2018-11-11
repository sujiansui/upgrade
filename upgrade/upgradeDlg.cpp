
// upgradeDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "upgrade.h"
#include "upgradeDlg.h"
#include "afxdialogex.h"
#include "Setting.h"
#include "global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CupgradeDlg 对话框

CSerialPort m_Comm;
CString m_filename;
CWinThread *pThreadUpgrade;
bool isThreadPause;

CupgradeDlg::CupgradeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_UPGRADE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CupgradeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CupgradeDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	
	ON_COMMAND(ID_MENU_SETTING, &CupgradeDlg::OnMenuSetting)
	ON_BN_CLICKED(IDCANCEL, &CupgradeDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CupgradeDlg::OnBnClickedOk)
	ON_COMMAND(ID_MENU_FILE, &CupgradeDlg::OnMenuFile)
	ON_MESSAGE(WM_COMM_RXCHAR, &CupgradeDlg::OnCommRxchar)
	ON_MESSAGE(WM_COMM_RXSTR, &CupgradeDlg::OnReceiveStr)
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

	com_set.port = 6;
	com_set.baud = 9600;
	CMenu menu;
	menu.LoadMenuW(IDR_MENU);
	SetMenu(&menu);
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




	CDialogEx::OnCancel();
}

int CupgradeDlg::ReadFileData(unsigned char *rbuf, CString filename)
{
	return 0;
}

afx_msg LRESULT CupgradeDlg::OnCommRxchar(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

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


UINT ThreadUpgrade(LPVOID pParam)
{
	

	while (!isThreadPause)
	{

	}
	return 0;
}


int  WaitComAck(int nTimer)
{
	
	return 0;
}


void CupgradeDlg::OnSendMessage(unsigned char* str, int port, int str_len)
{
	CString str1((char*)str);

	AfxMessageBox(str1);
}

void CupgradeDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	
	if (m_Comm.InitPort(this->GetSafeHwnd(), com_set.port, com_set.baud))
	{

		m_Comm.StartMonitoring();
		
	}
	else
	{
		AfxMessageBox(_T("串口已被占用！"));
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

	sbuf[7] = 0x0a;
	sbuf[8] = 0x0a;
	sbuf[9] = 0x01;
	sbuf[10] = 0x01;

	sbuf[11] = LOBYTE(up_file.len);
	sbuf[12] = HIBYTE(up_file.len);
	memcpy(sbuf + 13, up_file.buf + up_file.len - 8, 8);
	fcs = 0;
	for (int i = 0;i < 21;i++)
		fcs += sbuf[i];
	sbuf[21] = fcs;
	//START UPGRADE
	
	m_Comm.WriteToPort(sbuf, 22);
#ifdef _SEND_DATA_WITH_SIGSLOT
	m_Comm.sendMessageSignal.connect(this, &CupgradeDlg::OnSendMessage);
#endif
	
	isThreadPause = false;
	pThreadUpgrade = AfxBeginThread(ThreadUpgrade, this);
	
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

	path = GetModuleDir();
	filter = "bin文件(*.bin)|*.bin|hex文件(*.hex)|*.hex|所有文件(*.*)|*.*||";
	CFileDialog dlg(TRUE, NULL, path, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);
	if (dlg.DoModal() == IDOK)
	{
		CString filepathname;
		filepathname = dlg.GetPathName();
		m_filename = filepathname;
	}
	else
		m_filename = "";

}

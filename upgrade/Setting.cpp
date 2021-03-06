// Setting.cpp: 实现文件
//

#include "stdafx.h"
#include "upgrade.h"
#include "Setting.h"
#include "afxdialogex.h"
#define _GLOBAL_H
#include "global.h"

int COM_BAUD[] = { 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 38400, 56000, 57600, 115200 };


// Setting 对话框
extern CStatusBar m_StatusBar;
IMPLEMENT_DYNAMIC(Setting, CDialogEx)

Setting::Setting(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SETTING, pParent)
	, m_str_port(_T(""))
	, m_str_baud(_T(""))
{

}

Setting::~Setting()
{
}

void Setting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBOX_PORT, m_intPort);
	DDX_Control(pDX, IDC_CBOX_BAUD, m_intBaudRate);
	DDX_CBString(pDX, IDC_CBOX_PORT, m_str_port);
	DDX_CBString(pDX, IDC_CBOX_BAUD, m_str_baud);
}


BEGIN_MESSAGE_MAP(Setting, CDialogEx)
	ON_BN_CLICKED(IDOK, &Setting::OnBnClickedOk)
END_MESSAGE_MAP()


// Setting 消息处理程序


BOOL Setting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	CString strCom[255];
	CString strTemp;
	CString temp;



	HANDLE hCom;
	for (int i = 0;i < 50;i++)
	{
		strTemp.Format(_T("\\\\.\\COM%d"), i + 1);
		hCom = CreateFile(strTemp, 0, 0, 0,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (INVALID_HANDLE_VALUE == hCom)
			continue;
		temp.Format(_T("COM%d"), i+1);
		strCom[i] = temp;
		CloseHandle(hCom);
	}
	for (int i = 0; i < 50; i++)
	{
		if(!strCom[i].IsEmpty())
			m_intPort.AddString((LPCTSTR)strCom[i]);
	}
	temp.Format(_T("COM%d"), com_set.port);

	if (!strCom[0].IsEmpty())
		m_intPort.SetCurSel(m_intPort.FindString(0, temp));
	else
		m_intPort.SetCurSel(-1);

	//添加波特率到下拉列表
	for (int i = 0; i < sizeof(COM_BAUD) / sizeof(int); i++)
	{
		temp.Format(_T("%d"), COM_BAUD[i]);
		m_intBaudRate.AddString((LPCTSTR)temp);
	}

	temp.Format(_T("%d"), 9600);
	m_intBaudRate.SetCurSel(m_intBaudRate.FindString(0, temp));


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void Setting::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); //可以从控件获得数据
	//GetDlgItem(IDC_CBOX_PORT)->GetWindowText(m_str_port);  //获得组合框中的选中内容
	if (!m_str_port.IsEmpty())
	{
		m_str_port.Delete(0, 3);
		com_set.port = _tstoi(m_str_port);
		com_set.baud = _ttoi(m_str_baud);
	}
	else
	{
		com_set.port = COMNULL;
		
	}
	CString strCom;

	strCom.Format(_T("COM%d"), com_set.port);
	m_StatusBar.SetPaneText(0, _T("串口: ") + strCom, 0);
	m_StatusBar.Invalidate();

	CDialogEx::OnOK();
}

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
	if (!strCom[0].IsEmpty())
		m_intPort.SetCurSel(m_intPort.FindString(0, strCom[0]));
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
	CString temp;
	int len;
	UpdateData(TRUE); //可以从控件获得数据
	//GetDlgItem(IDC_CBOX_PORT)->GetWindowText(m_str_port);  //获得组合框中的选中内容
	//从这里开始进行转化，这是一个宏定义
	USES_CONVERSION;
	//进行转换 cstring --> char *
	char* keyChar = T2A(m_str_port.GetBuffer(0));
	m_str_port.ReleaseBuffer();

	len = m_str_port.GetLength();
	if (len == 4)
		temp = m_str_port.Mid(3, 1);
	else
		temp = m_str_port.Mid(3, 2);


	
	set.port = _ttoi(temp);
	set.baud = _ttoi(m_str_baud);
	CDialogEx::OnOK();
}

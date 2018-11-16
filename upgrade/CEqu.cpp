// CEqu.cpp: 实现文件
//

#include "stdafx.h"
#include "upgrade.h"
#include "CEqu.h"
#define _GLOBAL_H
#include "global.h"
#include "afxdialogex.h"


// CEqu 对话框

IMPLEMENT_DYNAMIC(CEqu, CDialogEx)

CEqu::CEqu(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_EQU, pParent)
	,m_equ(0)
{

}

CEqu::~CEqu()
{
}

void CEqu::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_TK_SMALL, m_equ);
}


BEGIN_MESSAGE_MAP(CEqu, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_TK_SMALL, &CEqu::OnBnClickedRadioTkSmall)
	ON_BN_CLICKED(IDC_RADIO_TK_BIG, &CEqu::OnBnClickedRadioTkSmall)
	ON_BN_CLICKED(IDOK, &CEqu::OnBnClickedOk)
END_MESSAGE_MAP()


// CEqu 消息处理程序


BOOL CEqu::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//CButton *pradio = (CButton*)GetDlgItem(IDC_RADIO_TK_SMALL);
	//pradio->SetCheck(true);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CEqu::OnBnClickedRadioTkSmall()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	switch (m_equ)
	{
	case 0:
		m_equ = 0;
		break;
	case 1:
		m_equ = 1;
		break;
	default:
		m_equ = 0;
		break;
	}
}


//void CEqu::OnBnClickedRadioTkBig()
//{
//	// TODO: 在此添加控件通知处理程序代码
//}


void CEqu::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	equ.type = m_equ;
	CDialogEx::OnOK();
}

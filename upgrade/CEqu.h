#pragma once


// CEqu 对话框

class CEqu : public CDialogEx
{
	DECLARE_DYNAMIC(CEqu)

public:
	CEqu(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CEqu();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_EQU };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_equ;
	afx_msg void OnBnClickedRadioTkSmall();
//	afx_msg void OnBnClickedRadioTkBig();
	afx_msg void OnBnClickedOk();
};

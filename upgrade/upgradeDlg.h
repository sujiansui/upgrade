
// upgradeDlg.h: 头文件
//

#pragma once

#include "SerialPort.h"

using namespace std;
using namespace itas109;

extern CSerialPort m_Comm;


// CupgradeDlg 对话框
class CupgradeDlg : public CDialogEx
{
// 构造
public:
	CupgradeDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UPGRADE_DIALOG };
#endif
	

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	void OnSendMessage(unsigned char* str, int port, int str_len);

public:
	
	BOOL m_ComAction;
	int m_intPort;
	int m_intBaudRate;
	int m_intDataBits;
	char m_cParity;    // 0 ,1,2 
	int m_intStopBits;    // 0=1 1=1.5 2=2

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnCommRxchar(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	
	
	afx_msg void OnMenuSetting();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnMenuFile();

private:
	int ReadFileData(unsigned char *rbuf, CString filename);
};

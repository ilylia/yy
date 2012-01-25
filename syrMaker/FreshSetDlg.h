#pragma once

#include "resource.h"


// CFreshSetDlg 对话框

class CFreshSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CFreshSetDlg)

public:
	CFreshSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFreshSetDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_FRESHSET };

public:
// 	void setType(int type);
// 	void setSandBoxie(BOOL sb);
// 	void setSandBoxieName(CString str);
// 
// 	int getType();
// 	BOOL useSandBoxie();
// 	CString getSandBoxieName();

protected:
// 	int _iYYType;				// yy句柄类型，0:YY主界面  1:YY设置界面
// 	BOOL _bSandBoxie;
// 	CString _strSBName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckSandboxie();
	afx_msg void OnBnClickedCheckYymain();
	afx_msg void OnBnClickedCheckYysetup();
};

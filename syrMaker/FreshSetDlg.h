#pragma once

#include "resource.h"


// CFreshSetDlg �Ի���

class CFreshSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CFreshSetDlg)

public:
	CFreshSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFreshSetDlg();

// �Ի�������
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
// 	int _iYYType;				// yy������ͣ�0:YY������  1:YY���ý���
// 	BOOL _bSandBoxie;
// 	CString _strSBName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckSandboxie();
	afx_msg void OnBnClickedCheckYymain();
	afx_msg void OnBnClickedCheckYysetup();
};

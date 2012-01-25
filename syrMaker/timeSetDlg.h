#pragma once

#include "resource.h"

// CtimeSetDlg 对话框

class CtimeSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CtimeSetDlg)

public:
	CtimeSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CtimeSetDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_TIMESET };

public:
	int m_iMM;
	int m_iSS;
	int m_iMS;
	int m_iMSMS;

	int m_time;

	void setTime(int time);
	int getTime();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedRadioMsm();
	afx_msg void OnBnClickedRadioMs();
	afx_msg void OnBnClickedOk();
};

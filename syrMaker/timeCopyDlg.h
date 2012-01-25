#pragma once

#include "resource.h"

// CtimeCopyDlg �Ի���

class CtimeCopyDlg : public CDialog
{
	DECLARE_DYNAMIC(CtimeCopyDlg)

public:
	CtimeCopyDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CtimeCopyDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_TIMECOPY };

public:
	int m_iMM;
	int m_iSS;
	int m_iMS;
	int m_iMSMS;

	int m_time;
	int m_isTo;

	void setTime(int time);
	int getTime();
	int isCopyTo();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedRadioCopyto();
	afx_msg void OnBnClickedRadioTimeoffset();
	afx_msg void OnBnClickedRadioMsm();
	afx_msg void OnBnClickedRadioMs();
	afx_msg void OnBnClickedOk();
};

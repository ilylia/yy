#pragma once

#include "resource.h"
#include "HyperLink.h"
#include "afxwin.h"

// CAboutDlg �Ի���

#define APP_NAME _T("YY��ʽ��Ļ�� for YY3.9���ϰ汾 ")
#define APP_VERSION _T("v1.0")
#define APP_BINTIME _T("2012.01.29 16:00")

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

public:
	CHyperLink m_L69120;
	CHyperLink m_L8095;
	CHyperLink m_Lzimuzu;

	CStatic m_ver;
	CStatic m_bintime;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

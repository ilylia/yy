#pragma once

#include "resource.h"
#include "HyperLink.h"
#include "afxwin.h"

// CAboutDlg 对话框

#define APP_NAME _T("花弄影字幕器 ")
#define APP_VERSION _T("v1.0")
#define APP_BINTIME _T("2014.01.16 20:00")

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

public:
	CHyperLink m_L69120;
	CHyperLink m_L8095;
	CHyperLink m_Lzimuzu;

	CStatic m_ver;
	CStatic m_bintime;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

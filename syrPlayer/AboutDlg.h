#pragma once

#include "resource.h"
#include "HyperLink.h"
#include "afxwin.h"

// CAboutDlg 对话框

#define APP_NAME _T("YY花式字幕器 for YY3.9以上版本 ")
#define APP_VERSION _T("v1.0.0.0")
#define APP_BINTIME _T(" 2012.01.28 23:00")

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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CStatic m_ver;
};

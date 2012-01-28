// AboutDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AboutDlg.h"

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LINK_69120, m_L69120);
	DDX_Control(pDX, IDC_LINK_8095, m_L8095);
	DDX_Control(pDX, IDC_LINK_ZIMUZU, m_Lzimuzu);
	DDX_Control(pDX, IDC_STATIC_VER, m_ver);
	DDX_Control(pDX, IDC_STATIC_BINTIME, m_bintime);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_L69120.SetURL(_T("yy://join:room_id=6968677"));
// 	m_L69120.SetLinkCursor(AfxGetApp()->LoadCursor(IDC_HAND));
	m_L8095.SetURL(_T("yy://join:room_id=61961377"));
// 	m_L8095.SetLinkCursor(AfxGetApp()->LoadCursor(IDC_HAND));
	m_Lzimuzu.SetURL(_T("yy://join:room_id=62380660"));
// 	m_Lzimuzu.SetLinkCursor(AfxGetApp()->LoadCursor(IDC_HAND));

	CString str = APP_NAME;
	str += APP_VERSION;
	m_ver.SetWindowText(str);

	m_bintime.SetWindowText(APP_BINTIME);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// FreshSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FreshSetDlg.h"
#include "Config.h"


// CFreshSetDlg 对话框

IMPLEMENT_DYNAMIC(CFreshSetDlg, CDialog)

CFreshSetDlg::CFreshSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFreshSetDlg::IDD, pParent)
{
// 	_bYYmain = TRUE;
// 	_bYYsetup = FALSE;
// 	_iYYType = 0;
// 	_bSandBoxie = FALSE;
// 	_strSBName = _T("");
}

CFreshSetDlg::~CFreshSetDlg()
{
}

void CFreshSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFreshSetDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CFreshSetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_SANDBOXIE, &CFreshSetDlg::OnBnClickedCheckSandboxie)
	ON_BN_CLICKED(IDC_CHECK_YYMAIN, &CFreshSetDlg::OnBnClickedCheckYymain)
	ON_BN_CLICKED(IDC_CHECK_YYSETUP, &CFreshSetDlg::OnBnClickedCheckYysetup)
END_MESSAGE_MAP()


// CFreshSetDlg 消息处理程序

BOOL CFreshSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
// 	_iYYType = config.m_iYYType;
// 	_bSandBoxie = config.m_bUseSBox;
// 	_strSBName = config.m_strSBoxName;

// 	((CButton*)GetDlgItem(IDC_CHECK_YYMAIN))->SetCheck(!_iYYType);
// 	((CButton*)GetDlgItem(IDC_CHECK_YYSETUP))->SetCheck(_iYYType);
// 	((CButton*)GetDlgItem(IDC_CHECK_SANDBOXIE))->SetCheck(_bSandBoxie);
// 	GetDlgItem(IDC_EDIT_SBNAME)->EnableWindow(_bSandBoxie);
// 	GetDlgItem(IDC_EDIT_SBNAME)->SetWindowText(_strSBName);

	((CButton*)GetDlgItem(IDC_CHECK_YYMAIN))->SetCheck(!config.m_iYYType);
	((CButton*)GetDlgItem(IDC_CHECK_YYSETUP))->SetCheck(config.m_iYYType);
	((CButton*)GetDlgItem(IDC_CHECK_SANDBOXIE))->SetCheck(config.m_bUseSBox);
	GetDlgItem(IDC_EDIT_SBNAME)->EnableWindow(config.m_bUseSBox);
	GetDlgItem(IDC_EDIT_SBNAME)->SetWindowText(config.m_strSBoxName);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// void CFreshSetDlg::setType(int type)
// {
// 	_iYYType = type;
// }
// 
// int CFreshSetDlg::getType()
// {
// 	return _iYYType;
// }
// 
// void CFreshSetDlg::setSandBoxie(BOOL sb)
// {
// 	_bSandBoxie = sb;
// }
// 
// BOOL CFreshSetDlg::useSandBoxie()
// {
// 	return _bSandBoxie;
// }
// 
// void CFreshSetDlg::setSandBoxieName(CString str)
// {
// 	_strSBName = str;
// }
// 
// CString CFreshSetDlg::getSandBoxieName()
// {
// 	return _strSBName;
// }

void CFreshSetDlg::OnBnClickedCheckYymain()
{
	config.m_iYYType = !config.m_iYYType;
	config.m_bEdit = TRUE;
	((CButton*)GetDlgItem(IDC_CHECK_YYMAIN))->SetCheck(!config.m_iYYType);
	((CButton*)GetDlgItem(IDC_CHECK_YYSETUP))->SetCheck(config.m_iYYType);
}

void CFreshSetDlg::OnBnClickedCheckYysetup()
{
	config.m_iYYType = !config.m_iYYType;
	config.m_bEdit = TRUE;
	((CButton*)GetDlgItem(IDC_CHECK_YYMAIN))->SetCheck(!config.m_iYYType);
	((CButton*)GetDlgItem(IDC_CHECK_YYSETUP))->SetCheck(config.m_iYYType);
}

void CFreshSetDlg::OnBnClickedCheckSandboxie()
{
	config.m_bUseSBox = ((CButton*)GetDlgItem(IDC_CHECK_SANDBOXIE))->GetCheck();
	config.m_bEdit = TRUE;
	GetDlgItem(IDC_EDIT_SBNAME)->EnableWindow(config.m_bUseSBox);
}

void CFreshSetDlg::OnBnClickedOk()
{
	config.m_bUseSBox = ((CButton*)GetDlgItem(IDC_CHECK_SANDBOXIE))->GetCheck();
	GetDlgItem(IDC_EDIT_SBNAME)->GetWindowText(config.m_strSBoxName);
	config.m_bEdit = TRUE;

	OnOK();
}

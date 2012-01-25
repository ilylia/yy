// FreshSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "syrPlayer.h"
#include "FreshSetDlg.h"


// CFreshSetDlg 对话框

IMPLEMENT_DYNAMIC(CFreshSetDlg, CDialog)

CFreshSetDlg::CFreshSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFreshSetDlg::IDD, pParent)
{
	_iYYType = 0;
// 	_bYYmain = TRUE;
// 	_bYYsetup = FALSE;
	_bSandBoxie = FALSE;
	_strSBName = _T("");
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
	((CButton*)GetDlgItem(IDC_CHECK_YYMAIN))->SetCheck(!_iYYType);
	((CButton*)GetDlgItem(IDC_CHECK_YYSETUP))->SetCheck(_iYYType);
	((CButton*)GetDlgItem(IDC_CHECK_SANDBOXIE))->SetCheck(_bSandBoxie);
	GetDlgItem(IDC_EDIT_SBNAME)->EnableWindow(_bSandBoxie);
	GetDlgItem(IDC_EDIT_SBNAME)->SetWindowText(_strSBName);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CFreshSetDlg::setType(int type)
{
	_iYYType = type;
}

int CFreshSetDlg::getType()
{
	return _iYYType;
}

void CFreshSetDlg::setSandBoxie(BOOL sb)
{
	_bSandBoxie = sb;
}

BOOL CFreshSetDlg::useSandBoxie()
{
	return _bSandBoxie;
}

void CFreshSetDlg::setSandBoxieName(CString str)
{
	_strSBName = str;
}

CString CFreshSetDlg::getSandBoxieName()
{
	return _strSBName;
}

void CFreshSetDlg::OnBnClickedCheckYymain()
{
	_iYYType = !_iYYType;
	((CButton*)GetDlgItem(IDC_CHECK_YYMAIN))->SetCheck(!_iYYType);
	((CButton*)GetDlgItem(IDC_CHECK_YYSETUP))->SetCheck(_iYYType);
}

void CFreshSetDlg::OnBnClickedCheckYysetup()
{
	_iYYType = !_iYYType;
	((CButton*)GetDlgItem(IDC_CHECK_YYMAIN))->SetCheck(!_iYYType);
	((CButton*)GetDlgItem(IDC_CHECK_YYSETUP))->SetCheck(_iYYType);
}

void CFreshSetDlg::OnBnClickedCheckSandboxie()
{
	_bSandBoxie = ((CButton*)GetDlgItem(IDC_CHECK_SANDBOXIE))->GetCheck();
	GetDlgItem(IDC_EDIT_SBNAME)->EnableWindow(_bSandBoxie);
}

void CFreshSetDlg::OnBnClickedOk()
{
	_bSandBoxie = ((CButton*)GetDlgItem(IDC_CHECK_SANDBOXIE))->GetCheck();
	GetDlgItem(IDC_EDIT_SBNAME)->GetWindowText(_strSBName);

	OnOK();
}

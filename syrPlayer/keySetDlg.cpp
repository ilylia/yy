// ketSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "syrPlayer.h"
#include "syrPlayerDlg.h"
#include "keySetDlg.h"


UINT vCode[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 
	'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
	VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6, VK_F7, VK_F8, VK_F9, VK_F10, VK_F11, VK_F12, 
	VK_HOME, VK_END, VK_LEFT, VK_UP, VK_RIGHT, VK_DOWN
};

CString vKey[] = {
	_T("A"), _T("B"), _T("C"), _T("D"), _T("E"), _T("F"), _T("G"), 
	_T("H"), _T("I"), _T("J"), _T("K"), _T("L"), _T("M"), _T("N"), 
	_T("O"), _T("P"), _T("Q"), _T("R"), _T("S"), _T("T"), 
	_T("U"), _T("V"), _T("W"), _T("X"), _T("Y"), _T("Z"),
	_T("0"), _T("1"), _T("2"), _T("3"), _T("4"), _T("5"), _T("6"), _T("7"), _T("8"), _T("9"), 
	_T("F1"), _T("F2"), _T("F3"), _T("F4"), _T("F5"), _T("F6"), _T("F7"), _T("F8"), 
	_T("F9"), _T("F10"), _T("F11"), _T("F12"), 
	_T("HOME"), _T("END"), _T("LEFT"), _T("UP"), _T("RIGHT"), _T("DOWN")
};

// CkeySetDlg 对话框

IMPLEMENT_DYNAMIC(CkeySetDlg, CDialog)

CkeySetDlg::CkeySetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CkeySetDlg::IDD, pParent)
{
	m_pKeySet = NULL;
}

CkeySetDlg::~CkeySetDlg()
{
}

void CkeySetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CkeySetDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CkeySetDlg::OnBnClickedOk)
// 	ON_BN_CLICKED(IDC_CHECK_USEKEY, &CkeySetDlg::OnBnClickedCheckUsekey)
END_MESSAGE_MAP()


// CkeySetDlg 消息处理程序

BOOL CkeySetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	initCombox();

	setKey();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// void CkeySetDlg::setUseKey(BOOL use)
// {
// 	m_bUseKey = use;
// }
// 
// BOOL CkeySetDlg::getUseKey()
// {
// 	return m_bUseKey;
// }

void CkeySetDlg::setKeySet(SKeySet* pKeySet)
{
	m_pKeySet = pKeySet;
}

SKeySet* CkeySetDlg::getKeySet()
{
	return m_pKeySet;
}

int CkeySetDlg::getIndex(UINT code)
{
	for (int i=0; i<54; i++)
	{
		if (code == vCode[i])
			return i;
	}
	return -1;
}

int CkeySetDlg::getIndex(CString key)
{
	for (int i=0; i<54; i++)
	{
		if (key == vKey[i])
			return i;
	}
	return -1;
}

int CkeySetDlg::setKey()
{
	if (m_pKeySet == NULL)
		return 0;

	((CButton*)GetDlgItem(IDC_CHECK_CTRL_BEGIN))->SetCheck(m_pKeySet[KEYSET_BEGIN].bCtrl);
	((CButton*)GetDlgItem(IDC_CHECK_SHIFT_BEGIN))->SetCheck(m_pKeySet[KEYSET_BEGIN].bShift);
	((CButton*)GetDlgItem(IDC_CHECK_ALT_BEGIN))->SetCheck(m_pKeySet[KEYSET_BEGIN].bAlt);
	((CComboBox*)GetDlgItem(IDC_COMBO_BEGIN))->SetCurSel(getIndex(m_pKeySet[KEYSET_BEGIN].uCode));

	((CButton*)GetDlgItem(IDC_CHECK_CTRL_STOP))->SetCheck(m_pKeySet[KEYSET_STOP].bCtrl);
	((CButton*)GetDlgItem(IDC_CHECK_SHIFT_STOP))->SetCheck(m_pKeySet[KEYSET_STOP].bShift);
	((CButton*)GetDlgItem(IDC_CHECK_ALT_STOP))->SetCheck(m_pKeySet[KEYSET_STOP].bAlt);
	((CComboBox*)GetDlgItem(IDC_COMBO_STOP))->SetCurSel(getIndex(m_pKeySet[KEYSET_STOP].uCode));

	((CButton*)GetDlgItem(IDC_CHECK_CTRL_FORWARD))->SetCheck(m_pKeySet[KEYSET_FORWARD].bCtrl);
	((CButton*)GetDlgItem(IDC_CHECK_SHIFT_FORWARD))->SetCheck(m_pKeySet[KEYSET_FORWARD].bShift);
	((CButton*)GetDlgItem(IDC_CHECK_ALT_FORWARD))->SetCheck(m_pKeySet[KEYSET_FORWARD].bAlt);
	((CComboBox*)GetDlgItem(IDC_COMBO_FORWARD))->SetCurSel(getIndex(m_pKeySet[KEYSET_FORWARD].uCode));

	((CButton*)GetDlgItem(IDC_CHECK_CTRL_BACKWARD))->SetCheck(m_pKeySet[KEYSET_BACKWARD].bCtrl);
	((CButton*)GetDlgItem(IDC_CHECK_SHIFT_BACKWARD))->SetCheck(m_pKeySet[KEYSET_BACKWARD].bShift);
	((CButton*)GetDlgItem(IDC_CHECK_ALT_BACKWARD))->SetCheck(m_pKeySet[KEYSET_BACKWARD].bAlt);
	((CComboBox*)GetDlgItem(IDC_COMBO_BACKWARD))->SetCurSel(getIndex(m_pKeySet[KEYSET_BACKWARD].uCode));

	return 1;
}

int CkeySetDlg::getKey()
{
	if (m_pKeySet == NULL)
		return 0;

	m_pKeySet[KEYSET_BEGIN].bCtrl = ((CButton*)GetDlgItem(IDC_CHECK_CTRL_BEGIN))->GetCheck();
	m_pKeySet[KEYSET_BEGIN].bShift = ((CButton*)GetDlgItem(IDC_CHECK_SHIFT_BEGIN))->GetCheck();
	m_pKeySet[KEYSET_BEGIN].bAlt = ((CButton*)GetDlgItem(IDC_CHECK_ALT_BEGIN))->GetCheck();
	m_pKeySet[KEYSET_BEGIN].uCode = vCode[((CComboBox*)GetDlgItem(IDC_COMBO_BEGIN))->GetCurSel()];

	m_pKeySet[KEYSET_STOP].bCtrl = ((CButton*)GetDlgItem(IDC_CHECK_CTRL_STOP))->GetCheck();
	m_pKeySet[KEYSET_STOP].bShift = ((CButton*)GetDlgItem(IDC_CHECK_SHIFT_STOP))->GetCheck();
	m_pKeySet[KEYSET_STOP].bAlt = ((CButton*)GetDlgItem(IDC_CHECK_ALT_STOP))->GetCheck();
	m_pKeySet[KEYSET_STOP].uCode = vCode[((CComboBox*)GetDlgItem(IDC_COMBO_STOP))->GetCurSel()];

	m_pKeySet[KEYSET_FORWARD].bCtrl = ((CButton*)GetDlgItem(IDC_CHECK_CTRL_FORWARD))->GetCheck();
	m_pKeySet[KEYSET_FORWARD].bShift = ((CButton*)GetDlgItem(IDC_CHECK_SHIFT_FORWARD))->GetCheck();
	m_pKeySet[KEYSET_FORWARD].bAlt = ((CButton*)GetDlgItem(IDC_CHECK_ALT_FORWARD))->GetCheck();
	m_pKeySet[KEYSET_FORWARD].uCode = vCode[((CComboBox*)GetDlgItem(IDC_COMBO_FORWARD))->GetCurSel()];

	m_pKeySet[KEYSET_BACKWARD].bCtrl = ((CButton*)GetDlgItem(IDC_CHECK_CTRL_BACKWARD))->GetCheck();
	m_pKeySet[KEYSET_BACKWARD].bShift = ((CButton*)GetDlgItem(IDC_CHECK_SHIFT_BACKWARD))->GetCheck();
	m_pKeySet[KEYSET_BACKWARD].bAlt = ((CButton*)GetDlgItem(IDC_CHECK_ALT_BACKWARD))->GetCheck();
	m_pKeySet[KEYSET_BACKWARD].uCode = vCode[((CComboBox*)GetDlgItem(IDC_COMBO_BACKWARD))->GetCurSel()];

	return 1;
}

void CkeySetDlg::initCombox()
{
	CComboBox* p = 0;

	p = (CComboBox*)GetDlgItem(IDC_COMBO_BEGIN);
	for (int i=0; i<54; i++)
		p->AddString(vKey[i]);

	p = (CComboBox*)GetDlgItem(IDC_COMBO_STOP);
	for (int i=0; i<54; i++)
		p->AddString(vKey[i]);

	p = (CComboBox*)GetDlgItem(IDC_COMBO_FORWARD);
	for (int i=0; i<54; i++)
		p->AddString(vKey[i]);

	p = (CComboBox*)GetDlgItem(IDC_COMBO_BACKWARD);
	for (int i=0; i<54; i++)
		p->AddString(vKey[i]);
}

CString CkeySetDlg::codeToKey(UINT code)
{
	int idx = getIndex(code);
	if (idx < 0 || idx > 53)
		return _T("");
	else
		return vKey[code];
}

UINT CkeySetDlg::keyToCode(CString key)
{
	int idx = getIndex(key);
	if (idx < 0 || idx > 53)
		return 0;
	else
		return vCode[idx];
}

// void CkeySetDlg::OnBnClickedCheckUsekey()
// {
// 	m_bUseKey = ((CButton*)GetDlgItem(IDC_CHECK_USEKEY))->GetCheck();
// 	for (UINT id=IDC_CHECK_CTRL_FRESH; id<=IDC_COMBO_BACKWARD; id++)
// 		GetDlgItem(id)->EnableWindow(m_bUseKey);
// }

void CkeySetDlg::OnBnClickedOk()
{
	getKey();

	CsyrPlayerDlg* dlg = (CsyrPlayerDlg*)GetParent();
	dlg->unregHotKey();
	dlg->regHotKey();

	OnOK();
}

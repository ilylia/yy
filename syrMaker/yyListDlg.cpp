// yyListDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "syrMaker.h"
#include "yyListDlg.h"
#include "FreshSetDlg.h"
#include "Config.h"


// CyyListDlg 对话框

IMPLEMENT_DYNAMIC(CyyListDlg, CDialog)

CyyListDlg::CyyListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CyyListDlg::IDD, pParent)
{
// 	m_iYYType = 1;
// 	m_bUseSBox = FALSE;
// 	m_strSBoxName = _T("");
}

CyyListDlg::~CyyListDlg()
{
}

void CyyListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_YY, m_lYY);
}


BEGIN_MESSAGE_MAP(CyyListDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_YYL_FRESH, &CyyListDlg::OnBnClickedFresh)
	ON_BN_CLICKED(IDC_BTN_YYL_FRESHSET, &CyyListDlg::OnBnClickedButtonFreshset)
	ON_BN_CLICKED(IDC_BTN_YYL_UP, &CyyListDlg::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BTN_YYL_DOWN, &CyyListDlg::OnBnClickedButtonDown)
	ON_BN_CLICKED(IDC_BTN_YYL_DEL, &CyyListDlg::OnBnClickedButtonDel)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CyyListDlg 消息处理程序

BOOL CyyListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// YY列表
	m_lYY.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_lYY.InsertColumn(0, _T("帐号"), LVCFMT_LEFT, 80);
	m_lYY.InsertColumn(1, _T("昵称"), LVCFMT_LEFT, 200);
	m_lYY.InsertColumn(2, _T("签名"), LVCFMT_LEFT, 200);

// 	m_iYYType = config.m_iYYType;
// 	m_bUseSBox = config.m_bUseSBox;
// 	m_strSBoxName = config.m_strSBoxName;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

int CyyListDlg::setNick(int idx, CString nick)
{
	// 	if (nick == _T(""))
	// 		return 0;

	int cn = (int)m_sYY.size();
// 	if (cn < 1)
// 		return 0;
	if (cn <= idx)
		return 0;

	CString str;
// 	delStrBlank(nick);
	if (nick.IsEmpty())
	{
		return 0;
	}

// 	// 过滤
// 	vector<CString>::iterator it = m_vFilter.begin();
// 	for (; it!= m_vFilter.end(); it++)
// 	{
// 		if (nick.Find(*it) != -1)
// 			return 0;
// 	}

// 	if (m_iLrcType == 1)
// 	{
// 		CString strPrefix, strPostfix;
// 		m_cPrefix.GetWindowText(strPrefix);
// 		m_cPostfix.GetWindowText(strPostfix);
// 		str = strPrefix;
// 		if (m_isAlign && cn > 1)
// 		{
// 			int n = 20 - lstrlen(str) - lstrlen(nick);
// 			if (n < 0)
// 			{
// 				;
// 			}
// 			else
// 			{
// 				CString tt(_T(''), m_iCurYY*n/(cn-1));//　//
// 				str += tt;
// 			}
// 		}
// 		str += nick;
// 		str += strPostfix;
// 		m_iCurYY++;
// 		m_iCurYY = m_iCurYY % cn;
// 	}
// 	else if (m_iLrcType == 2 || m_iLrcType == 3)
// 	{
// 		str = nick;
// 	}
	str = nick;

	LPTSTR tmp = str.GetBuffer();
	if (config.m_iYYType == 0)
	{
		::SendMessage(m_sYY[idx].m_hEdit, WM_SETTEXT, 0, (LPARAM)tmp);
		::PostMessage(m_sYY[idx].m_hEdit, WM_KEYDOWN, VK_RETURN, 0);
	}
	else if (config.m_iYYType == 1)
	{
		::SendMessage(m_sYY[idx].m_hEditNick, WM_SETTEXT, 0, (LPARAM)tmp);
		::SendMessage(m_sYY[idx].m_hBtnApply, WM_LBUTTONDOWN, 0, 0);
		::SendMessage(m_sYY[idx].m_hBtnApply, WM_LBUTTONUP, 0, 0);
	}

	return 1;
}

int CyyListDlg::setNick()
{
	vector<SYYInfo>::iterator it = m_sYY.begin();
	for (; it!=m_sYY.end(); it++)
	{
		LPTSTR tmp = it->m_sNick.GetBuffer();
		if (config.m_iYYType == 0)
		{
			::SendMessage(it->m_hEdit, WM_SETTEXT, 0, (LPARAM)tmp);
			::PostMessage(it->m_hEdit, WM_KEYDOWN, VK_RETURN, 0);
		}
		else if (config.m_iYYType == 1)
		{
			::SendMessage(it->m_hEditNick, WM_SETTEXT, 0, (LPARAM)tmp);
			::SendMessage(it->m_hBtnApply, WM_LBUTTONDOWN, 0, 0);
			::SendMessage(it->m_hBtnApply, WM_LBUTTONUP, 0, 0);
		}
	}
	return 1;
}

int CyyListDlg::getYYFromMain(CString strClassName)
{
	HWND hWndLabel = NULL;
	int idx = m_lYY.GetItemCount();
	CString winClass = strClassName + _T("LabelEditWnd");
	do 
	{
		hWndLabel = ::FindWindowEx(NULL, hWndLabel, winClass, NULL);
		if (hWndLabel == NULL)
		{
			break;
		}
		HWND hEdit = ::FindWindowEx(hWndLabel, NULL, _T("Edit"), NULL);
		if (hEdit == NULL)
		{
			break;
		}
		HWND hWndParent = ::GetParent(hWndLabel);
		if (hWndParent == NULL)
		{
			break;
		}
		HWND hEditNick = ::FindWindowEx(hWndParent, NULL, _T("Static"), NULL);
		if (hEditNick == NULL)
		{
			continue;
		}
		HWND hEditSign = ::FindWindowEx(hWndParent, hEditNick, _T("Static"), NULL);
		if (hEditSign == NULL)
		{
			continue;
		}

		SYYInfo yys;
		yys.m_hWndParent = hWndParent;
		yys.m_hEditNick  = hEditNick;
		yys.m_hEditSign  = hEditSign;
		yys.m_hLabelEdit = hWndLabel;
		yys.m_hEdit      = hEdit;

		TCHAR text[1024];
		::SendMessage(hWndParent, WM_GETTEXT, 1024, (LPARAM)text);
		m_lYY.InsertItem(idx, text);
		::SendMessage(hEditNick, WM_GETTEXT, 1024, (LPARAM)text);
		m_lYY.SetItemText(idx, 1, text);
		yys.m_sNick = text;
		::SendMessage(hEditSign, WM_GETTEXT, 1024, (LPARAM)text);
		m_lYY.SetItemText(idx, 2, text);
		idx++;

		m_sYY.push_back(yys);
	} while (hWndLabel != NULL);

	return 1;
}

int CyyListDlg::getYYFromSetup(CString strClassName)
{
	HWND hWndParent = NULL;
	int idx = m_lYY.GetItemCount();
	CString winClass = strClassName + _T("my setup");
	CString applyClass = strClassName + _T("AfxWnd70u");
	do 
	{
		hWndParent = ::FindWindowEx(NULL, hWndParent, winClass, _T("设置"));
		if (hWndParent == NULL)
		{
			break;
		}
		HWND hEditName = ::FindWindowEx(hWndParent, NULL, _T("Edit"), NULL);
		if (hEditName == NULL)
		{
			continue;
		}
		HWND hEditNick = ::FindWindowEx(hWndParent, hEditName, _T("Edit"), NULL);
		if (hEditNick == NULL)
		{
			continue;
		}
		HWND hEditSign = ::FindWindowEx(hWndParent, hEditNick, _T("Edit"), NULL);
		if (hEditSign == NULL)
		{
			continue;
		}
		HWND hBtnApply = ::FindWindowEx(hWndParent, NULL, applyClass, _T("应用"));
		if (hBtnApply == NULL)
		{
			continue;
		}

		SYYInfo yys;
		yys.m_hWndParent = hWndParent;
		yys.m_hEditName = hEditName;
		yys.m_hEditNick = hEditNick;
		yys.m_hEditSign = hEditSign;
		yys.m_hBtnApply = hBtnApply;

		TCHAR text[1024];
		::SendMessage(hEditName, WM_GETTEXT, 1024, (LPARAM)text);
		m_lYY.InsertItem(idx, text);
		::SendMessage(hEditNick, WM_GETTEXT, 1024, (LPARAM)text);
		m_lYY.SetItemText(idx, 1, text);
		yys.m_sNick = text;
		::SendMessage(hEditSign, WM_GETTEXT, 1024, (LPARAM)text);
		m_lYY.SetItemText(idx, 2, text);
		idx++;

		m_sYY.push_back(yys);
	} while (hWndParent != NULL);

	return 1;
}

void CyyListDlg::OnBnClickedFresh()
{
	m_sYY.clear();
	m_lYY.DeleteAllItems();

	if (config.m_iYYType == 0)
	{
		getYYFromMain(_T(""));
		if (config.m_bUseSBox)
		{
			CString str;
			str.Format(_T("Sandbox:%s:"), config.m_strSBoxName);
			getYYFromMain(str);
		}
	}
	else if (config.m_iYYType == 1)
	{
		getYYFromSetup(_T(""));
		if (config.m_bUseSBox)
		{
			CString str;
			str.Format(_T("Sandbox:%s:"), config.m_strSBoxName);
			getYYFromSetup(str);
		}
	}

	if (m_sYY.size() == 0)
	{
		MessageBox(_T("no yy"));
		return;
	}
}

void CyyListDlg::OnBnClickedButtonFreshset()
{
	CFreshSetDlg dlg;
// 	dlg.setType(m_iYYType);
// 	dlg.setSandBoxie(m_bUseSBox);
// 	dlg.setSandBoxieName(m_strSBoxName);
	if (dlg.DoModal() == IDOK)
	{
// 		m_iYYType = dlg.getType();
// 		m_bUseSBox = dlg.useSandBoxie();
// 		m_strSBoxName = dlg.getSandBoxieName();
	}
}

void CyyListDlg::OnBnClickedButtonUp()
{
	int cursel = m_lYY.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if(cursel < 1)
	{
		return;
	}

	CString name = m_lYY.GetItemText(cursel, 0);
	CString nick = m_lYY.GetItemText(cursel, 1);
	CString sign = m_lYY.GetItemText(cursel, 2);

	m_lYY.DeleteItem(cursel);
	m_lYY.InsertItem(cursel-1, name);
	m_lYY.SetItemText(cursel-1, 1, nick);
	m_lYY.SetItemText(cursel-1, 2, sign);
	m_lYY.SetItemState(cursel-1, LVIS_SELECTED, LVIS_SELECTED);

	SYYInfo syy = m_sYY[cursel];
	m_sYY.erase(m_sYY.begin() + cursel);
	m_sYY.insert(m_sYY.begin() + cursel - 1, syy);
}

void CyyListDlg::OnBnClickedButtonDown()
{
	int cursel = m_lYY.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if(cursel == -1 || cursel == m_lYY.GetItemCount()-1)
	{
		return;
	}

	CString name = m_lYY.GetItemText(cursel, 0);
	CString nick = m_lYY.GetItemText(cursel, 1);
	CString sign = m_lYY.GetItemText(cursel, 2);

	m_lYY.DeleteItem(cursel);
	m_lYY.InsertItem(cursel+1, name);
	m_lYY.SetItemText(cursel+1, 1, nick);
	m_lYY.SetItemText(cursel+1, 2, sign);
	m_lYY.SetItemState(cursel+1, LVIS_SELECTED, LVIS_SELECTED);

	SYYInfo syy = m_sYY[cursel];
	m_sYY.erase(m_sYY.begin() + cursel);
	m_sYY.insert(m_sYY.begin() + cursel + 1, syy);
}

void CyyListDlg::OnBnClickedButtonDel()
{
	int cursel = m_lYY.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if(cursel == -1)
	{
		return;
	}

	m_lYY.DeleteItem(cursel);

	m_sYY.erase(m_sYY.begin() + cursel);
}

void CyyListDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}

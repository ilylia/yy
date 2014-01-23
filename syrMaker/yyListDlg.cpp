// yyListDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "syrMaker.h"
#include "yyListDlg.h"
#include "Config.h"
#include "HtmlWndMgr.h"


// CyyListDlg 对话框

IMPLEMENT_DYNAMIC(CyyListDlg, CDialog)

CyyListDlg::CyyListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CyyListDlg::IDD, pParent)
{
}

CyyListDlg::~CyyListDlg()
{
	CHtmlWndMgr::GetInstance()->DelInstance();
}

void CyyListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_YY, m_lYY);
}


BEGIN_MESSAGE_MAP(CyyListDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_YYL_FRESH, &CyyListDlg::OnBnClickedFresh)
	ON_BN_CLICKED(IDC_BTN_YYL_HIDEYYAPP, &CyyListDlg::OnBnClickedButtonHideyyapp)
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

	//
	CHtmlWndMgr::GetInstance()->Init(this);
	CHtmlWndMgr::GetInstance()->Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CyyListDlg::AddYY(unsigned int imId)
{
	map<CString, VARIANT> userInfo;
	CHtmlWndMgr::GetInstance()->GetUserInfo(imId, userInfo);

	SYYInfo yyp;
	yyp.m_imId = imId;
	yyp.m_sNick = userInfo[_T("name")].bstrVal;
	yyp.m_sSign = userInfo[_T("sign")].bstrVal;
	m_sYY.push_back(yyp);

	int n = m_lYY.GetItemCount();
	CString strimId;
	strimId.Format(_T("%d"), yyp.m_imId);
	m_lYY.InsertItem(n, strimId);
	m_lYY.SetItemText(n, 1, yyp.m_sNick);
	m_lYY.SetItemText(n, 2, yyp.m_sSign);
}

void CyyListDlg::DelYY(unsigned int imId)
{
	int cursel = 0;
	for (vector<SYYInfo>::iterator itr = m_sYY.begin(); itr != m_sYY.end(); ++itr)
	{
		if (itr->m_imId == imId)
		{
			m_sYY.erase(itr);
			break;
		}
		++cursel;
	}
	m_lYY.DeleteItem(cursel);
}

void CyyListDlg::SendNick(SYYInfo& syp, CString str)
{
	CHtmlWndMgr::GetInstance()->Rename(syp.m_imId, str);
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

	str = nick;

	SendNick(m_sYY[idx], str);

	return 1;
}

int CyyListDlg::setNick()
{
	vector<SYYInfo>::iterator it = m_sYY.begin();
	for (; it!=m_sYY.end(); it++)
	{
		SendNick(*it, it->m_sNick);
	}
	return 1;
}

void CyyListDlg::OnBnClickedFresh()
{
	CHtmlWndMgr::GetInstance()->Refresh();
}

void CyyListDlg::OnBnClickedButtonHideyyapp()
{
	BOOL bShow = CHtmlWndMgr::GetInstance()->ToggleYYAppVisible();

	CString btnText = bShow ? _T("隐藏YY应用窗口") : _T("显示YY应用窗口");
	CWnd* btn = GetDlgItem(IDC_BTN_YYL_HIDEYYAPP);
	if (btn != NULL)
	{
		btn->SetWindowText(btnText);
	}
}

void CyyListDlg::OnBnClickedButtonUp()
{
	int cursel = m_lYY.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if(cursel < 1)
	{
		return;
	}

	SYYInfo syy = m_sYY[cursel];
	m_sYY.erase(m_sYY.begin() + cursel);
	m_sYY.insert(m_sYY.begin() + cursel - 1, syy);

	CString name = m_lYY.GetItemText(cursel, 0);
	CString nick = m_lYY.GetItemText(cursel, 1);
	CString sign = m_lYY.GetItemText(cursel, 2);

	m_lYY.DeleteItem(cursel);
	m_lYY.InsertItem(cursel-1, name);
	m_lYY.SetItemText(cursel-1, 1, nick);
	m_lYY.SetItemText(cursel-1, 2, sign);
	m_lYY.SetItemState(cursel-1, LVIS_SELECTED, LVIS_SELECTED);
}

void CyyListDlg::OnBnClickedButtonDown()
{
	int cursel = m_lYY.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if(cursel == -1 || cursel == m_lYY.GetItemCount()-1)
	{
		return;
	}

	SYYInfo syy = m_sYY[cursel];
	m_sYY.erase(m_sYY.begin() + cursel);
	m_sYY.insert(m_sYY.begin() + cursel + 1, syy);

	CString name = m_lYY.GetItemText(cursel, 0);
	CString nick = m_lYY.GetItemText(cursel, 1);
	CString sign = m_lYY.GetItemText(cursel, 2);

	m_lYY.DeleteItem(cursel);
	m_lYY.InsertItem(cursel+1, name);
	m_lYY.SetItemText(cursel+1, 1, nick);
	m_lYY.SetItemText(cursel+1, 2, sign);
	m_lYY.SetItemState(cursel+1, LVIS_SELECTED, LVIS_SELECTED);
}

void CyyListDlg::OnBnClickedButtonDel()
{
	int cursel = m_lYY.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if(cursel == -1)
	{
		return;
	}

	vector<SYYInfo>::iterator itr = m_sYY.begin() + cursel;
	CHtmlWndMgr::GetInstance()->OnDelYY(itr->m_imId);

	m_sYY.erase(itr);

	m_lYY.DeleteItem(cursel);
}

void CyyListDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}

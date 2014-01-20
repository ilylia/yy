// ListCtrlEditable.cpp : 实现文件
//

#include "stdafx.h"
#include "ListCtrlEditable.h"
#include "syrPlayerDlg.h"

// CComboxNoBorder

IMPLEMENT_DYNAMIC(CComboxNoBorder, CComboBox)
CComboxNoBorder::CComboxNoBorder()
{
}

CComboxNoBorder::~CComboxNoBorder()
{
}


BEGIN_MESSAGE_MAP(CComboxNoBorder, CComboBox)
	//ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()



// CComboxNoBorder 消息处理程序


void CComboxNoBorder::OnPaint()
{
	CPaintDC dc(this); // device context for painting

}

HBRUSH CComboxNoBorder::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	CRect rc;
	if (nCtlColor == CTLCOLOR_EDIT)
	{	
		CEdit *pEdit = (CEdit*)pWnd;
		if (pEdit)
		{
			GetClientRect(&rc);
			rc.right -= GetSystemMetrics(SM_CXVSCROLL) + 2;
			pEdit->MoveWindow(&rc);
			pEdit->GetClientRect(&rc);
			pEdit->SetRect(&rc);
		}
	}

	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}

void CComboxNoBorder::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if (bShow)
	{
		LOGFONT lf;
		GetFont()->GetLogFont(&lf);
		SendMessage(CB_SETITEMHEIGHT, -1, abs(lf.lfHeight));
	}
	CComboBox::OnShowWindow(bShow, nStatus);	
}

////////////////////////////////////////////////////////////////////////////////////

// CListCtrlEditable

IMPLEMENT_DYNAMIC(CListCtrlEditable, CListCtrl)
CListCtrlEditable::CListCtrlEditable()
{
}

CListCtrlEditable::~CListCtrlEditable()
{
}


BEGIN_MESSAGE_MAP(CListCtrlEditable, CListCtrl)
	ON_WM_CREATE()
	//ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnLvnItemchanged)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnNMDblclk)	
	ON_EN_KILLFOCUS(501, OnCtrlKillFocus)
	ON_CBN_KILLFOCUS(502, OnCtrlKillFocus)
	ON_LBN_KILLFOCUS(503, OnCtrlKillFocus)
	ON_NOTIFY(NM_KILLFOCUS, 504, OnCtrlKillFocus)
END_MESSAGE_MAP()



// CListCtrlEditable 消息处理程序
BOOL CListCtrlEditable::PreCreateWindow(CREATESTRUCT& cs)
{
	//cs.dwExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;

	return CListCtrl::PreCreateWindow(cs);
}

int CListCtrlEditable::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, ControlType ctrltype, DWORD style, bool bEditable, int nFormat, int nWidth1, int nSubItem)
{
	int ival = CListCtrl::InsertColumn(nCol, lpszColumnHeading, nFormat, nWidth1, nSubItem);
	if (ival != -1)
		m_ctMap.insert(std::pair<int, CColumType>((int)m_ctMap.size(),  CColumType(nCol, nSubItem, ctrltype, style, bEditable, this)));
	return ival;
}

CWnd* CListCtrlEditable::GetColumControl(int col /* = -1 */, int subitem /* = -1 */)
{
	CWnd *pwnd = NULL;
	for (int i = 0; i < (int)m_ctMap.size(); i++)
	{
		CColumType &ct = m_ctMap[i];
		if (ct.iCol == col && ct.iSubItem == subitem)
		{
			pwnd = ct.pCtrlWnd;
			break;
		}
	}
	return pwnd;
}

void CListCtrlEditable::SetColumCtrlText(CWnd *pColumCtrl)
{
	if (pColumCtrl)
	{
		for (int i = 0; i < (int)m_ctMap.size(); i++)
		{
			CColumType &ct = m_ctMap[i];
			if (ct.pCtrlWnd == pColumCtrl)
			{
				if (ct.ctCntrlType == CtrlType_Edit)
				{
					pColumCtrl->SetWindowText(m_CurCell.sText);
					CEdit* et = (CEdit*)pColumCtrl;
					et->SetSel(0, -1);
				}
				else if (ct.ctCntrlType == CtrlType_ComboBox)
				{
					CComboBox *pcmb = (CComboBox*)pColumCtrl;
					CString sitem;
					int i = 0;
					for (i = 0; i < pcmb->GetCount(); i++)
					{
						pcmb->GetLBText(i, sitem);
						if (sitem == m_CurCell.sText)
							break;
					}
					if (i < pcmb->GetCount())
						pcmb->SetCurSel(i);
				}
				else if (ct.ctCntrlType == CtrlType_ListBox)
				{
					CListBox *plb = (CListBox*)pColumCtrl;
					CString sitem;
					int i = 0;
					for (i; i < plb->GetCount(); i++)
					{
						plb->GetText(i, sitem);
						if (sitem == m_CurCell.sText)
							break;
					}
					if (i < plb->GetCount())
						plb->SetCurSel(i);
				}
				else if (ct.ctCntrlType == CtrlType_DateTimePicker)
				{
					pColumCtrl->SetWindowText(m_CurCell.sText);
				}
				//else if (ct.ctCntrlType == CtrlType_Ip)
				//{
				//	pColumCtrl->SetWindowText(m_CurCell.sText);
				//}
				break;
			}
		}
	}
}

CString CListCtrlEditable::GetColumCtrlText(int col /* = -1 */, int subitem /* = -1 */)
{
	CString sVal = m_CurCell.sText;
	CWnd *pColumCtrl = GetColumControl(col, subitem);
	if (pColumCtrl)
	{
		for (int i = 0; i < (int)m_ctMap.size(); i++)
		{
			CColumType &ct = m_ctMap[i];
			if (ct.pCtrlWnd == pColumCtrl)
			{
				if (ct.ctCntrlType == CtrlType_Edit)
				{
					pColumCtrl->GetWindowText(sVal);
				}
				else if (ct.ctCntrlType == CtrlType_ComboBox)
				{
					CComboBox *pcmb = (CComboBox*)pColumCtrl;
					if (pcmb->GetCurSel() > -1)
						pcmb->GetLBText(pcmb->GetCurSel(), sVal);
				}
				else if (ct.ctCntrlType == CtrlType_ListBox)
				{
					CListBox *plb = (CListBox*)pColumCtrl;
					if (plb->GetCurSel() > -1)
						plb->GetText(plb->GetCurSel(), sVal);
				}
				else if (ct.ctCntrlType == CtrlType_DateTimePicker)
				{
					CIPAddressCtrl *pIp = (CIPAddressCtrl*)pColumCtrl;
					pIp->GetWindowText(sVal);
				}
				//else if (ct.ctCntrlType == CtrlType_Ip)
				//{
				//	pColumCtrl->SetWindowText(sVal);
				//}
				break;
			}
		}
	}
	
	return sVal;
}

BOOL CListCtrlEditable::ShowColumCtrl(int nItem /* = -1 */, int nSubItem /* = -1 */)
{
	BOOL ret = EnsureVisible(nItem, FALSE);

	// 保存单元格数据
	m_CurCell.iRow = nItem;
	m_CurCell.iCol = nSubItem;
	m_CurCell.sText = GetItemText(nItem, nSubItem);

	CWnd *pColCtrl = GetColumControl(nSubItem, -1);
	if (pColCtrl)
	{
		CRect rect;
		GetSubItemRect(nItem, nSubItem, LVIR_LABEL, rect);
		CRect rtList;
		GetClientRect(&rtList);
		if(rect.right > rtList.Width()) 
			Scroll(CSize( rect.Width() > rtList.Width()?rect.left : rect.right - rtList.Width(), 0));
		if(rect.left < 0) 
			Scroll(CSize(rect.left));

		GetSubItemRect(nItem, nSubItem, LVIR_LABEL, rect);
		rect.right = min(rect.right, rtList.Width()-4);
		pColCtrl->MoveWindow(&rect);
		pColCtrl->ShowWindow(SW_SHOW);		
		pColCtrl->SendMessage(WM_SHOWWINDOW, TRUE);
		pColCtrl->SetFocus();

		SetColumCtrlText(pColCtrl);
	}

	return ret;
}

void CListCtrlEditable::HideAllColumCtrl()
{
	//SetCurCellText(m_CurCell);

	for (int i = 0; i < (int)m_ctMap.size(); i++)
	{
		CColumType &ct = m_ctMap[i];
		if (ct.pCtrlWnd)
		{
			if (ct.pCtrlWnd->IsWindowVisible())
				ct.pCtrlWnd->ShowWindow(SW_HIDE);
		}
	}
}

void CListCtrlEditable::SetCurCellText(CCell &cell)
{
	if (cell.iCol >= 0 && cell.iRow >= 0)
	{
		CWnd *pColCtrl = GetColumControl(cell.iCol, -1);
		if (pColCtrl)
		{
			cell.sText = GetColumCtrlText(cell.iCol, -1);
			CListCtrl::SetItemText(cell.iRow, cell.iCol, cell.sText);

			//////////////////////////////////// to parent
			CsyrPlayerDlg* pw = (CsyrPlayerDlg*)GetParent();
			if (pw != NULL)
			{
				pw->modifyYY(cell.iRow, cell.iCol, cell.sText);
			}
		}
	}
}

void CListCtrlEditable::OnCtrlKillFocus()
{
	SetCurCellText(m_CurCell);
	HideAllColumCtrl();
}

void CListCtrlEditable::OnCtrlKillFocus( NMHDR * pNotifyStruct, LRESULT * result )
{
	SetCurCellText(m_CurCell);
	HideAllColumCtrl();
}

int CListCtrlEditable::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}

void CListCtrlEditable::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	HideAllColumCtrl();

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
}

void CListCtrlEditable::OnItemChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	HideAllColumCtrl();
}

void CListCtrlEditable::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	HideAllColumCtrl();

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	if(!pNMLV) 
		return;
	int nItem = pNMLV->iItem, 
		nSubItem = pNMLV->iSubItem;
	// 显示控件
	ShowColumCtrl(nItem, nSubItem);
}

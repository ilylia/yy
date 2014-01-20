#pragma once

#include <map>


// CComboxNoBorder
class CComboxNoBorder : public CComboBox
{
	/*
 * Combobox information
 */
	DECLARE_DYNAMIC(CComboxNoBorder)

public:
	CComboxNoBorder();
	virtual ~CComboxNoBorder();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};

//////////////////////////////////////////////////////////////////////////////////////////

// CListCtrlEditable

class CListCtrlEditable : public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlEditable)
public:
	// ��ɫ
	typedef enum mycolor{
		Color_White = RGB(255, 255, 255),
		Color_Black = RGB(0, 0, 0),
	};

	// ֧�ֵı༭�ؼ�
	typedef enum controltype{
		CtrlType_Static = 0,			// ��̬�ı���
		//CtrlType_Button,				// ��ť
		CtrlType_Edit,					// �ɱ༭��
		CtrlType_ComboBox,				// ��Ͽ�
		CtrlType_ListBox,				// �б��
		CtrlType_DateTimePicker,		// ���ڿؼ�
		//CtrlType_Ip,					// IP�ؼ�
	} ControlType;

	// ��¼��ǰ��Ԫ����Ϣ
	class CCell
	{
	public:
		CCell() : sText(""), iRow(-1), iCol(-1), dwBkColor(Color_White), dwFontColor(Color_Black)
		{

		}
		CCell(CString text, int row = -1, int col = -1, DWORD bkcolor = Color_White, DWORD fontcolor = Color_Black) : 
			  sText(text)
			, iRow(row)
			, iCol(col)
			, dwBkColor(bkcolor)
			, dwFontColor(fontcolor)
		{

		}

		CString sText;
		int		iCol,
				iRow;
		DWORD	dwBkColor,
				dwFontColor;
	};

	// �е�������Ϣ
	class CColumType
	{
	public:
		CColumType(int col = 0, int subitem = -1, ControlType ct = CtrlType_Static, DWORD style = 0, BOOL editable = false, CWnd *pparent = NULL) : 
		  iCol(col), iSubItem(subitem), ctCntrlType(ct), dwStyle(style), bEditable(editable), pCtrlWnd(NULL), pParent(pparent)
		{
			CreateControl();
		}

		CColumType(const CColumType &ct)
		{
			iCol = ct.iCol;
			iSubItem = ct.iSubItem;
			ctCntrlType = ct.ctCntrlType;
			dwStyle = ct.dwStyle;
			bEditable = ct.bEditable;
			pParent = ct.pParent;
			pCtrlWnd = NULL;
			CreateControl();
		}

		virtual ~CColumType()
		{
			if (pCtrlWnd != NULL)
			{
				delete pCtrlWnd;
				pCtrlWnd = NULL;
			}
		}

		CColumType& operator=(const CColumType &ct)
		{
			iCol = ct.iCol;
			iSubItem = ct.iSubItem;
			ctCntrlType = ct.ctCntrlType;
			dwStyle = ct.dwStyle;
			bEditable = ct.bEditable;
			pParent = ct.pParent;
			if (pCtrlWnd)
			{
				delete pCtrlWnd;
				pCtrlWnd = NULL;
			}
			CreateControl();
		}

		int			iCol,		// �к�
					iSubItem;	// ���к�
	
		ControlType ctCntrlType;
		DWORD		dwStyle;	// �ؼ���ʽ
		BOOL		bEditable;	// �Ƿ�ɱ༭
		CWnd	    *pCtrlWnd;	// �ؼ�ָ��
		CWnd		*pParent;

	private:
		void CreateControl()
		{
			// �������ʹ�����Ӧ�Ŀؼ�
			if (bEditable == TRUE)
			{
				if (ctCntrlType == CtrlType_Edit)
				{
					CEdit *pCtrl = new CEdit();
					pCtrl->Create(dwStyle | WS_CHILD | WS_TABSTOP | WS_BORDER, CRect(0, 0, 0, 0), pParent, 501);
					pCtrl->SetFont(pParent->GetFont());
					pCtrlWnd = pCtrl;
				}
				else if (ctCntrlType == CtrlType_ComboBox)
				{
					CComboxNoBorder *pCtrl = new CComboxNoBorder();
					pCtrl->Create(dwStyle | WS_CHILD | WS_TABSTOP | CBS_DROPDOWNLIST, CRect(10,10,400,400), pParent, 502);
					pCtrl->SetFont(pParent->GetFont());
					pCtrlWnd = pCtrl;
				}
				else if (ctCntrlType == CtrlType_ListBox)
				{
					CListBox *pCtrl = new CListBox();
					pCtrl->Create(dwStyle | WS_CHILD | WS_TABSTOP | WS_BORDER, CRect(0,0,200,200), pParent, 503);
					pCtrl->SetFont(pParent->GetFont());
					pCtrlWnd = pCtrl;
				}
				else if (ctCntrlType == CtrlType_DateTimePicker)
				{
					CDateTimeCtrl *pCtrl = new CDateTimeCtrl();
					pCtrl->Create(dwStyle | WS_CHILD | WS_TABSTOP | WS_BORDER,	CRect(0, 0, 0, 0), pParent, 504);
					pCtrl->SetFont(pParent->GetFont());
					pCtrlWnd = pCtrl;
				}
				//else if (ctCntrlType == CtrlType_Ip)
				//{
				//	CIPAddressCtrl *pCtrl = new CIPAddressCtrl();
				//	pCtrl->Create(dwStyle | WS_CHILD | WS_TABSTOP,	CRect(0, 0, 0, 0), pParent, 505);
				//	pCtrl->SetFont(pParent->GetFont());
				//	pCtrlWnd = pCtrl;
				//}
			}
		}
	};

protected:
	typedef std::map<int, CColumType>		mapColumType;
	mapColumType		m_ctMap;
	CCell				m_CurCell;

public:
	CListCtrlEditable();
	virtual ~CListCtrlEditable();

	// ������ ctrltypeָ���е�����, styleָ���ؼ�����ʽ(WS_CHILD��),��������ͬMSDN��clistctrl::InsertColumn��˵��
	int InsertColumn(int nCol,	
					 LPCTSTR lpszColumnHeading,	
					 ControlType ctrltype = CtrlType_Static,  // �пؼ�����
					 DWORD style = 0,						  // �пؼ���ʽ
					 bool bEditable = false,				  // ���Ƿ�ɱ༭
					 int nFormat = LVCFMT_LEFT,	
					 int nWidth = -1, 
					 int nSubItem = -1);
	// �õ���Ӧ��ͷ�Ŀؼ�ָ�룬������ʱ����NULL
	CWnd* GetColumControl(int col = -1, int subitem = -1);

	void OnItemChanged(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	DECLARE_MESSAGE_MAP()

	BOOL ShowColumCtrl(int item = -1, int subitem = -1);
	void HideAllColumCtrl();
	void SetColumCtrlText(CWnd *pColumCtrl = NULL);
	CString GetColumCtrlText(int col = -1, int subitem = -1);
	void SetCurCellText(CCell &cell);

	afx_msg void OnCtrlKillFocus();
	afx_msg void OnCtrlKillFocus(NMHDR * pNotifyStruct=NULL, LRESULT * result = NULL);

	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};




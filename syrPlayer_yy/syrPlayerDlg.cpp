// YYnickDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "syrPlayer.h"
#include "syrPlayerDlg.h"
#include "AboutDlg.h"
#include "tinyxml/tinyxml.h"
#include "YYApiMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ID_TIMER_SYR 998
#define ID_TIMER_LRC 999
#define ID_TIMER_STOP 1000

// CsyrPlayerDlg 对话框

CsyrPlayerDlg::CsyrPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CsyrPlayerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_lrc = NULL;

	m_iTimePos = 0;
}

CsyrPlayerDlg::~CsyrPlayerDlg()
{
	CYYApiMgr::GetInstance()->Exit();
}

void CsyrPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_LRCCON, m_treeLrcCon);
	DDX_Control(pDX, IDC_STATIC_TIME, m_cTime);
	DDX_Control(pDX, IDC_SLIDER_TIME, m_sliderTime);
}

BEGIN_MESSAGE_MAP(CsyrPlayerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_WM_TIMER()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_BEGIN, &CsyrPlayerDlg::OnBnClickedBtnBegin)
	ON_BN_CLICKED(IDC_BTN_STOP, &CsyrPlayerDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_FORWARD, &CsyrPlayerDlg::OnBnClickedBtnForward)
	ON_BN_CLICKED(IDC_BTN_BACKWARD, &CsyrPlayerDlg::OnBnClickedBtnBackward)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_LRCCON, &CsyrPlayerDlg::OnTvnSelchangedTreeLrccon)
	ON_NOTIFY(NM_CLICK, IDC_TREE_LRCCON, &CsyrPlayerDlg::OnNMClickTreeLrccon)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TREE_LRCCON, &CsyrPlayerDlg::OnNMCustomdrawTreeLrccon)
	ON_WM_NCHITTEST()
	ON_WM_NCRBUTTONUP()
END_MESSAGE_MAP()


// CsyrPlayerDlg 消息处理程序

BOOL CsyrPlayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CString str = APP_NAME;
	str += APP_VERSION;
	SetWindowText(str);

	// 时间
	CFont font;
	font.CreatePointFont(720,_T("Arial"));    //720为字体的大小
	m_cTime.SetFont(&font);
	m_sliderTime.SetRangeMin(0);
	m_sliderTime.EnableWindow(FALSE);

	// 初始状态，开始禁用
	GetDlgItem(IDC_BTN_BEGIN)->EnableWindow(FALSE);

	// yy接口
	if(!CYYApiMgr::GetInstance()->Init())
	{
		MessageBox(_T("初始化失败！！！"));
	}
	m_yyNick = CYYApiMgr::GetInstance()->GetUserNick();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CsyrPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CsyrPlayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CsyrPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CsyrPlayerDlg::OnNcHitTest(CPoint point)
{
	CRect re;
	GetClientRect(&re);

	ScreenToClient(&point);
	if (point.y >= 0 && point.y <= re.Height())
		return HTCAPTION;
	else
		return CDialog::OnNcHitTest(point);
}

void CsyrPlayerDlg::OnNcRButtonUp(UINT nHitTest, CPoint point)
{
	CRect re;
	GetClientRect(&re);
	ScreenToClient(&point);
	if (nHitTest==HTCAPTION && re.PtInRect(point))
		SendMessage(WM_RBUTTONUP, (WPARAM)m_hWnd, MAKELPARAM(point.x, point.y));
	else
		CDialog::OnNcRButtonUp(nHitTest, point);
}

void CsyrPlayerDlg::OnDropFiles(HDROP hDropInfo)
{
	TCHAR   szFilePathName[_MAX_PATH+1] = {0};
	UINT  nNumOfFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0); //得到文件个数
	if (nNumOfFiles > 0)
	{
		DragQueryFile(hDropInfo, 0, szFilePathName, _MAX_PATH);  //得到文件名

		stopLRC();
		initList(szFilePathName);
	}
	DragFinish(hDropInfo);

	CDialog::OnDropFiles(hDropInfo);
}

void CsyrPlayerDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER_TIME)
	{
		m_iTimePos = m_sliderTime.GetPos();
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

int CsyrPlayerDlg::initList(CString strLrcFile)
{
	//////////////////////////////////////////////////////////////////////////
	int iRet = 0;
	CString ext = strLrcFile.Right(3);
	ext.MakeLower();
	if (ext == _T("lrc"))
	{
		m_iLrcType = 1;

		LRC_INFO_T* lrc = GetLRCFile(strLrcFile.GetBuffer(), &iRet);
		if (iRet != 0)
			return 0;

		DESTROY_LRC(&m_lrc);
		m_lrc = lrc;

		m_treeLrcCon.DeleteAllItems();
		m_sliderTime.SetPos(0);
		m_sliderTime.EnableWindow(TRUE);

		CString tmp;
		for (int i=0; i<m_lrc->num_t; i++)
		{
			int min = m_lrc->tlrc[i].time/60000;
			int sec = m_lrc->tlrc[i].time%60000 / 1000;
			int mss = m_lrc->tlrc[i].time%1000 / 10;
			tmp.Format(_T("[%02d:%02d.%02d]"), min, sec, mss);
			tmp += m_lrc->slrc[m_lrc->tlrc[i].index].txt;
			HTREEITEM hItem = m_treeLrcCon.InsertItem(tmp);
			m_treeLrcCon.SetItemData(hItem, (DWORD_PTR)i);
		}

		m_iTimeAll = m_lrc->tlrc[m_lrc->num_t-1].time + 5000;
		m_sliderTime.SetRangeMax(m_iTimeAll);

		GetDlgItem(IDC_BTN_BEGIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_BEGIN)->SetWindowText(_T("开始"));

		return 1;
	}
	else if (ext == _T("txt"))		// ????
	{
		m_iLrcType = 2;

		vector<CString> txt;
		iRet = GetTXTFile(strLrcFile.GetBuffer(), txt);

		m_treeLrcCon.DeleteAllItems();
		m_sliderTime.SetPos(0);
		m_sliderTime.EnableWindow(FALSE);

		vector<CString>::iterator it = txt.begin();
		for (; it!=txt.end(); it++)
		{
			HTREEITEM hItem = m_treeLrcCon.InsertItem(*it);
		}

		GetDlgItem(IDC_BTN_BEGIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_BEGIN)->SetWindowText(_T("开始"));

		return 1;
	}
	else if (ext == _T("syr"))
	{
		m_iLrcType = 3;

		if (m_syr.Open(strLrcFile) == 0)
			return 0;

		GetDlgItem(IDC_BTN_BEGIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_BEGIN)->SetWindowText(_T("开始"));
		m_sliderTime.SetPos(0);
		m_sliderTime.EnableWindow(TRUE);
		m_sliderTime.SetRangeMax(m_syr.getMaxTime());

		//MessageBox(_T("请点击列表中的yy号来查看其字幕！"));
		return 1;
	}
	else
	{
		m_iLrcType = 0;
		return 0;
	}
}

int CsyrPlayerDlg::initList(int idx)
{
	if (idx < 0 || idx >= m_syr.getYYerCount())
		return 0;

	m_treeLrcCon.DeleteAllItems();

	vector<SLyric>& lrcs = m_syr.getLyrics(idx);
	CString tmp;
	int i = 0;
	vector<SLyric>::iterator it = lrcs.begin();
	for (; it!=lrcs.end(); it++)
	{
		int min = it->time/60000;
		int sec = it->time%60000 / 1000;
		int mss = it->time%1000 / 10;
		tmp.Format(_T("[%02d:%02d.%02d]"), min, sec, mss);
		tmp += it->str;
		HTREEITEM hItem = m_treeLrcCon.InsertItem(tmp);
		m_treeLrcCon.SetItemData(hItem, (DWORD_PTR)i);
	}

	return 1;
}

int CsyrPlayerDlg::startLRC()
{
	if (m_iLrcType == 1)
		SetTimer(ID_TIMER_LRC, 500, NULL);
	else if (m_iLrcType == 3)
	{
		//		int cny = (int)m_sYYP.size();
		//		int cnl = m_syr.getYYerCount();
		//		if (cny < cnl)
		//		{
		//			CString tmp;
		//			tmp.Format(_T("列表中的YY号%d个\n不够本字幕的要求%d个！"), cny, cnl);
		//			MessageBox(tmp);
		//			return 0;
		//		}
		//		else if (cny > cnl)
		//		{
		//			MessageBox(_T("列表中的YY号比需要的多~\n排在后面的号码将不会用上~"));
		//		}

		SetTimer(ID_TIMER_SYR, 100, NULL);
	}

	GetDlgItem(IDC_BTN_BEGIN)->SetWindowText(_T("暂停"));
	return 1;
}

int CsyrPlayerDlg::pauseLRC()
{
	if (m_iLrcType == 1)
		KillTimer(ID_TIMER_LRC);
	else if (m_iLrcType == 3)
	{
		KillTimer(ID_TIMER_SYR);
	}

	GetDlgItem(IDC_BTN_BEGIN)->SetWindowText(_T("开始"));
	return 1;
}

int CsyrPlayerDlg::stopLRC()
{
	if (m_iLrcType == 1)
	{
		KillTimer(ID_TIMER_LRC);
		HTREEITEM ht = m_treeLrcCon.GetSelectedItem();
		m_treeLrcCon.SetItemState(ht, 0, TVIS_SELECTED);
	}
	else if (m_iLrcType == 3)
	{
		KillTimer(ID_TIMER_SYR);
		m_syr.reset();
	}

	GetDlgItem(IDC_BTN_BEGIN)->SetWindowText(_T("开始"));
	m_iTimePos = 0;
	m_cTime.SetWindowText(_T("00:00"));
	m_sliderTime.SetPos(0);

	setNick();

	return 1;
}

int CsyrPlayerDlg::delStrBlank(CString& str)
{
	LPTSTR tmp = str.GetBuffer();

	int j = 0;
	while (tmp[j] && tmp[j]==_T(' '))
	{
		j++;
	}

	str = tmp + j;

	return 1;
}

void CsyrPlayerDlg::SendNick(CString str)
{
	CYYApiMgr::GetInstance()->Rename(str);
}

int CsyrPlayerDlg::setNick(int idx, CString nick)
{
	CString str;
	delStrBlank(nick);
	if (nick.IsEmpty())
	{
		return 0;
	}

	//// 过滤
	//vector<CString>::iterator it = m_vFilter.begin();
	//for (; it!= m_vFilter.end(); it++)
	//{
	//	if (nick.Find(*it) != -1)
	//		return 0;
	//}

 	if (m_iLrcType == 1)
 	{
 		str += nick;
 		//m_iCurYY++;
 		//m_iCurYY = m_iCurYY % cn;
 	}
 	else if (m_iLrcType == 2 || m_iLrcType == 3)
	{
		str = nick;
	}

	SendNick(str);

	return 1;
}

int CsyrPlayerDlg::setNick()
{
	SendNick(m_yyNick);
	return 1;
}


void CsyrPlayerDlg::OnNMClickTreeLrccon(NMHDR *pNMHDR, LRESULT *pResult)
{
// 	return;
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	if (pNMTreeView->itemNew.mask != 1)
		return;
	HTREEITEM hCurItem = pNMTreeView->itemNew.hItem;
// 	HTREEITEM hCurItem = m_treeLrcCon.GetSelectedItem();
// 	TVHITTESTINFO   p; 
// 	m_treeLrcCon.HitTest(&p);
// 	HTREEITEM hCurItem = p.hItem;

	if (hCurItem == NULL)
		return;

// 	m_iCurYY++;
// 	m_iCurYY = m_iCurYY % 2;

// 	CString strPrefix, strPostfix;
// // 	m_editPrefix.GetWindowText(strPrefix);
// // 	m_editPostfix.GetWindowText(strPostfix);
// 	m_cPrefix.GetWindowText(strPrefix);
// 	m_cPostfix.GetWindowText(strPostfix);

// 	CString lrcd;
	if (m_iLrcType == 1)
	{
		int i = (int)m_treeLrcCon.GetItemData(hCurItem);
		if (i > m_lrc->num_t)
			return;

		m_iTimePos = m_lrc->tlrc[i].time;
		m_sliderTime.SetPos(m_iTimePos);

		int min = m_lrc->tlrc[i].time/60000;
		int sec = m_lrc->tlrc[i].time%60000 / 1000;
		CString tm;
		tm.Format(_T("%02d:%02d"), min, sec);
		m_cTime.SetWindowText(tm);

// 		lrcd = m_lrc->slrc[m_lrc->tlrc[i].index].txt;
	}
// 	else if (m_iLrcType == 2)
// 	{
// 		lrcd = m_treeLrcCon.GetItemText(hCurItem);
// 	}
// 	else
// 	{
// 		return;
// 	}
// 
// 	// 过滤
// 	vector<CString>::iterator it = m_vFilter.begin();
// 	for (; it!= m_vFilter.end(); it++)
// 	{
// 		if (lrcd.Find(*it) != -1)
// 			return;
// 	}
// 
// 	CString str;
// 	LPWSTR tmp = lrcd.GetBuffer();
// 	int j = 0;
// 	while (tmp[j] && tmp[j]==_T(' '))
// 	{
// 		j++;
// 	}
// 	if (j < lstrlen(tmp))
// 	{
// 		//str = strPrefix;
// 		//str += tmp;
// 		//str += strPostfix;
// 		//setNick(str);
// 
// 		if (m_iCurYY != 0)
// 		{
// 			int n = 20 - lstrlen(tmp);
// 			CString tt(_T("　"), n);
// 			str = tt;
// 		}
// 		str = str + tmp;
// 		setNick(m_iCurYY, str);
// 	}

	*pResult = 0;
}

void CsyrPlayerDlg::OnTvnSelchangedTreeLrccon(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	HTREEITEM hCurItem = pNMTreeView->itemNew.hItem;
	// 	int i = (int)m_treeLrcCon.GetItemData(hCurItem);

	//int min = m_lrc->tlrc[i].time/60000;
	//int sec = m_lrc->tlrc[i].time%60000 / 1000;
	//CString tm;
	//tm.Format(_T("%02d:%02d"), min, sec);
	//m_cTime.SetWindowText(tm);

// 	CString str;
// 	LPWSTR tmp = m_lrc->slrc[m_lrc->tlrc[i].index].txt;
	CString lrcd;
	if (m_iLrcType == 1)
	{
		int i = (int)m_treeLrcCon.GetItemData(hCurItem);
		if (i > m_lrc->num_t)
			return;

// 		m_iTimePos = m_lrc->tlrc[i].time;

// 		int min = m_lrc->tlrc[i].time/60000;
// 		int sec = m_lrc->tlrc[i].time%60000 / 1000;
// 		int min = m_iTimePos / 60000;
// 		int sec = m_iTimePos % 60000 / 1000;
// 		CString tm;
// 		tm.Format(_T("%02d:%02d"), min, sec);
// 		m_cTime.SetWindowText(tm);

		lrcd = m_lrc->slrc[m_lrc->tlrc[i].index].txt;
	}
	else if (m_iLrcType == 2)
	{
		lrcd = m_treeLrcCon.GetItemText(hCurItem);
	}
	else
	{
		return;
	}

	setNick(m_iCurYY, lrcd);

	*pResult = 0;
}

void CsyrPlayerDlg::OnNMCustomdrawTreeLrccon(NMHDR *pNMHDR, LRESULT *pResult)
{
 	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
 	// TODO: 在此添加控件通知处理程序代码
 	*pResult = 0;

// 	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
// 	*pResult = CDRF_DODEFAULT;
// 
// 	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
// 	{
// 		*pResult = CDRF_NOTIFYITEMDRAW;
// 	}
// 	else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
// 	{
// 		*pResult = CDRF_NOTIFYSUBITEMDRAW;
// 	}
// 	else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
// 	{
// 		COLORREF clrNewTextColor, clrNewBkColor;
// 		int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec );
// 		POSITION pos = m_treeLrcCon.GetFirstSelectedItemPosition();
// 		int index = m_treeLrcCon.GetNextSelectedItem(pos);
// 		if (index == nItem)//如果要刷新的项为当前选择的项，则将文字设为白色，背景色设为蓝色
// 		{
// 			clrNewTextColor = RGB(255,255,255);        //Set the text to white
// 			clrNewBkColor = RGB(49,106,197);        //Set the background color to blue
// 		}
// 		else
// 		{
// 			clrNewTextColor = RGB(0,0,0);        //set the text black
// 			clrNewBkColor = RGB(255,255,255);    //leave the background color white
// 		}
// 
// 		pLVCD->clrText = clrNewTextColor;
// 		pLVCD->clrTextBk = clrNewBkColor;
// 		*pResult = CDRF_DODEFAULT;
// 	}
}

void CsyrPlayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == ID_TIMER_LRC)
	{
		m_iTimePos += 510;
		m_sliderTime.SetPos(m_iTimePos);

		int min = m_iTimePos/60000;
		int sec = m_iTimePos%60000 / 1000;
		CString tm;
		tm.Format(_T("%02d:%02d"), min, sec);
		m_cTime.SetWindowText(tm);

		int t_pos = m_iTimePos + m_lrc->offset + 500;
		int idx = 0;
		for (int i=0; i<m_lrc->num_t; i++)
		{
			if (t_pos>=m_lrc->tlrc[i].time && t_pos<m_lrc->tlrc[i+1].time)
			{
				idx = i;
				break;
			}
		}
// 		UINT8 idx = 0;
// 		if (t_pos < m_lrc->tlrc[0].time)
// 			idx = 0;
// 		//else if (t_pos >= m_lrc->tlrc[m_lrc->num_t-1].time)
// 		//	idx = 
// 		else
// 		{
// 			for (; idx<m_lrc->num_t; idx++)
// 			{
// 				if (t_pos>=m_lrc->tlrc[idx].time && t_pos<m_lrc->tlrc[idx+1].time)
// 				{
// 					break;
// 				}
// 			}
// 		}

		if (idx == 0)
		{
			HTREEITEM hFirst = m_treeLrcCon.GetRootItem();
			if (hFirst != NULL)
			{
				m_treeLrcCon.SelectItem(hFirst);
			}
		}
		else if (idx < m_lrc->num_t-1)
		{
			HTREEITEM hCur = m_treeLrcCon.GetSelectedItem();
			if (hCur != NULL)
			{
				int idxCur = (int)m_treeLrcCon.GetItemData(hCur);
				if (idx > idxCur)
				{
					HTREEITEM hNext = m_treeLrcCon.GetNextSiblingItem(hCur);
					while (hNext != NULL)
					{
						if (idx == m_treeLrcCon.GetItemData(hNext))
							break;
						hNext = m_treeLrcCon.GetNextSiblingItem(hNext);
					}
					if (hNext != NULL)
					{
						m_treeLrcCon.SelectItem(hNext);
					}
				}
				else if (idx < idxCur)
				{
					HTREEITEM hPrev = m_treeLrcCon.GetPrevSiblingItem(hCur);
					while (hPrev != NULL)
					{
						if (idx == m_treeLrcCon.GetItemData(hPrev))
							break;
						hPrev = m_treeLrcCon.GetPrevSiblingItem(hPrev);
					}
					if (hPrev != NULL)
					{
						m_treeLrcCon.SelectItem(hPrev);
					}
				}
			}
		}
		else if (idx = m_lrc->num_t-1)
		{
			HTREEITEM hCur = m_treeLrcCon.GetSelectedItem();
			if (hCur != NULL)
			{
				int idxCur = (int)m_treeLrcCon.GetItemData(hCur);
				if (idx > idxCur)
				{
					HTREEITEM hNext = m_treeLrcCon.GetNextSiblingItem(hCur);
					while (hNext != NULL)
					{
						if (idx == m_treeLrcCon.GetItemData(hNext))
							break;
						hNext = m_treeLrcCon.GetNextSiblingItem(hNext);
					}
					if (hNext != NULL)
					{
						m_treeLrcCon.SelectItem(hNext);
						SetTimer(ID_TIMER_STOP, 5000, NULL);
					}
				}
				else if (idx < idxCur)
				{
					HTREEITEM hPrev = m_treeLrcCon.GetPrevSiblingItem(hCur);
					while (hPrev != NULL)
					{
						if (idx == m_treeLrcCon.GetItemData(hPrev))
							break;
						hPrev = m_treeLrcCon.GetPrevSiblingItem(hPrev);
					}
					if (hPrev != NULL)
					{
						m_treeLrcCon.SelectItem(hPrev);
						SetTimer(ID_TIMER_STOP, 5000, NULL);
					}
				}
			}
		}
		else
		{
			KillTimer(ID_TIMER_LRC);
			GetDlgItem(IDC_BTN_BEGIN)->SetWindowText(_T("开始"));
			m_iTimePos = 0;
			m_cTime.SetWindowText(_T("00:00"));
			m_sliderTime.SetPos(m_iTimePos);
		}
	}
	else if (nIDEvent == ID_TIMER_SYR)
	{
		m_iTimePos += 110;
		m_sliderTime.SetPos(m_iTimePos);

		int min = m_iTimePos/60000;
		int sec = m_iTimePos%60000 / 1000;
		CString tm;
		tm.Format(_T("%02d:%02d"), min, sec);
		m_cTime.SetWindowText(tm);

		int t_pos = m_iTimePos/* + m_syr.getOffset() + 500*/;

		BOOL bStop = TRUE;
		int cn = m_syr.getYYerCount();
		for (int i=0; i<cn; i++)
		{
			int ret = m_syr.calcYYerLrc(i, t_pos);
			if (ret == 0)
				bStop = FALSE;
			else if (ret == 1)
			{
				bStop = FALSE;
				setNick(i, m_syr.getYYerCurLrc(i));
			}
		}

		if (bStop)
		{
			KillTimer(ID_TIMER_SYR);
			GetDlgItem(IDC_BTN_BEGIN)->SetWindowText(_T("开始"));
			m_iTimePos = 0;
			m_syr.reset();
			m_cTime.SetWindowText(_T("00:00"));
			m_sliderTime.SetPos(m_iTimePos);
			setNick();
		}
	}
	else if (nIDEvent == ID_TIMER_STOP)
	{
		KillTimer(ID_TIMER_STOP);
		KillTimer(ID_TIMER_LRC);
		GetDlgItem(IDC_BTN_BEGIN)->SetWindowText(_T("开始"));
		m_iTimePos = 0;
		m_cTime.SetWindowText(_T("00:00"));
		m_sliderTime.SetPos(m_iTimePos);
		setNick();
	}

	CDialog::OnTimer(nIDEvent);
}

void CsyrPlayerDlg::OnBnClickedBtnBegin()
{
	if (!GetDlgItem(IDC_BTN_BEGIN)->IsWindowEnabled())
		return;

	CString state;
	GetDlgItem(IDC_BTN_BEGIN)->GetWindowText(state);
	if (state == _T("暂停"))
	{
// 		KillTimer(ID_TIMER_LRC);
// 		GetDlgItem(IDC_BTN_BEGIN)->SetWindowText(_T("开始"));
		pauseLRC();
	}
	else if (state == _T("开始"))
	{
// 		SetTimer(ID_TIMER_LRC, 500, NULL);
// 		GetDlgItem(IDC_BTN_BEGIN)->SetWindowText(_T("暂停"));
		startLRC();
	}
}

void CsyrPlayerDlg::OnBnClickedBtnStop()
{
// 	KillTimer(ID_TIMER_LRC);
// 	GetDlgItem(IDC_BTN_BEGIN)->SetWindowText(_T("开始"));
// 	m_iTimePos = 0;
// 	m_cTime.SetWindowText(_T("00:00"));
	stopLRC();
}

void CsyrPlayerDlg::OnBnClickedBtnForward()
{
	m_iTimePos += 500;
	m_sliderTime.SetPos(m_iTimePos);
}

void CsyrPlayerDlg::OnBnClickedBtnBackward()
{
	m_iTimePos -= 500;
	m_sliderTime.SetPos(m_iTimePos);
}


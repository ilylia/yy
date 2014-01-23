// syrMakerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "syrMaker.h"
#include "syrMakerDlg.h"
#include "AboutDlg.h"
#include "timeCopyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ID_TIMER_SYR 998

// CsyrMakerDlg 对话框

CsyrMakerDlg::CsyrMakerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CsyrMakerDlg::IDD, pParent)
	, m_syrPath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	iYYerNumOld = 1;

	m_pYYListDlg = NULL;
	m_pFileListDlg = NULL;
}

CsyrMakerDlg::~CsyrMakerDlg()
{
	freeCtrl();

	// 
	delete m_vEdit[0];
	delete m_vButton[0];

	if (m_pYYListDlg != NULL)
		delete m_pYYListDlg;

	if (m_pFileListDlg != NULL)
		delete m_pFileListDlg;
}

void CsyrMakerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TIMEAXIS, m_timeAxis);
	DDX_Control(pDX, IDC_STATIC_TIME, m_cTime);
}

BEGIN_MESSAGE_MAP(CsyrMakerDlg, CDialog)
	//}}AFX_MSG_MAP
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_NEW, &CsyrMakerDlg::OnBnClickedButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CsyrMakerDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CsyrMakerDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_SAVEAS, &CsyrMakerDlg::OnBnClickedButtonSaveas)
	ON_BN_CLICKED(IDC_BTN_BEGIN, &CsyrMakerDlg::OnBnClickedBtnBegin)
	ON_BN_CLICKED(IDC_BTN_STOP, &CsyrMakerDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_FORWARD, &CsyrMakerDlg::OnBnClickedBtnForward)
	ON_BN_CLICKED(IDC_BTN_BACKWARD, &CsyrMakerDlg::OnBnClickedBtnBackward)
	ON_BN_CLICKED(IDC_BTN_STEPFORWARD, &CsyrMakerDlg::OnBnClickedBtnStepforward)
	ON_BN_CLICKED(IDC_BTN_STEPBACK, &CsyrMakerDlg::OnBnClickedBtnStepback)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CsyrMakerDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, &CsyrMakerDlg::OnBnClickedButtonAbout)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CsyrMakerDlg::OnBnClickedButtonDel)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON_BASE, IDC_BUTTON_BASE+48, &CsyrMakerDlg::OnBnClickedRangeDel)
	ON_COMMAND(ID_TIMEAXIS_COPY, &CsyrMakerDlg::OnTimeaxisCopy)
	ON_BN_CLICKED(IDC_BUTTON_YYLIST, &CsyrMakerDlg::OnBnClickedButtonYylist)
	ON_BN_CLICKED(IDC_BUTTON_FILELIST, &CsyrMakerDlg::OnBnClickedButtonFilelist)
	//ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_SYRMAKER_LOADLRCTP, &CsyrMakerDlg::OnSyrmakerLoadlrctp)
	ON_WM_NCHITTEST()
	ON_WM_NCRBUTTONUP()
END_MESSAGE_MAP()


// CsyrMakerDlg 消息处理程序

BOOL CsyrMakerDlg::OnInitDialog()
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

	CRect re;
	GetDlgItem(IDC_EDIT_REFER)->GetWindowRect(&re);
	ScreenToClient(&re);
	ClrcEdit* pEdit = new ClrcEdit;
	pEdit->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_EX_STATICEDGE, re, this, IDC_EDIT_BASE);
	pEdit->SetFont(GetFont());
	pEdit->SetLimitText(20);
	pEdit->setIndex((int)m_vEdit.size());

	GetDlgItem(IDC_BUTTON_REFER)->GetWindowRect(&re);
	ScreenToClient(&re);
	CButton* pButton = new CButton;
	pButton->Create(_T("-"), WS_CHILD | WS_VISIBLE | WS_TABSTOP/* | WS_EX_STATICEDGE*/, re, this, IDC_BUTTON_BASE);

	m_vStaticEdit.push_back((CStatic*)GetDlgItem(IDC_STATIC_EDIT_BASE));
	m_vEdit.push_back(pEdit);
	m_vButton.push_back(pButton);
	adjustPos();

	// 
	CRichEditCtrl* pe2 = (CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT21_TEXT);
	PARAFORMAT2 pf2;
	pf2.cbSize = sizeof(PARAFORMAT2);
	pf2.dwMask = PFM_LINESPACING;
	pf2.dyLineSpacing = 300;
	pf2.bLineSpacingRule = 4;
	pe2->SetParaFormat(pf2);

	// 
	m_timeAxis.init();
	m_timeAxis.setSyr(&m_syr);

	// 时间
	CFont font; 
	font.CreatePointFont(200,_T("隶书"));    //720为字体的大小
	m_cTime.SetFont(&font);

	if (!m_syrPath.IsEmpty())
	{
// 		openSyr();
		if (m_syr.Open(m_syrPath) != 0)
			resetYYer();
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CsyrMakerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CsyrMakerDlg::OnPaint()
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
HCURSOR CsyrMakerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CsyrMakerDlg::OnDropFiles(HDROP hDropInfo)
{
	TCHAR   szFilePathName[_MAX_PATH+1] = {0};
	UINT  nNumOfFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0); //得到文件个数
	if (nNumOfFiles > 0)
	{
		DragQueryFile(hDropInfo, 0, szFilePathName, _MAX_PATH);  //得到文件名

		m_syrPath = szFilePathName;
// 		openSyr();
		if (m_syr.Open(szFilePathName) == 0)
			return;

		resetYYer();
	}
	DragFinish(hDropInfo);
	CDialog::OnDropFiles(hDropInfo);
}

BOOL CsyrMakerDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_DELETE:		// 
			{
				CWnd* pWnd = FromHandle(pMsg->hwnd);
				if (!pWnd->IsKindOf(RUNTIME_CLASS(CEdit))
					&& pWnd != GetDlgItem(IDC_RICHEDIT21_TEXT))
				{
					m_timeAxis.delSelectTime();
					Invalidate();
				}
			}
			break;
		case VK_CONTROL:		// 
			{
				CWnd* pWnd = FromHandle(pMsg->hwnd);
				if (!pWnd->IsKindOf(RUNTIME_CLASS(CEdit))
					&& pWnd != GetDlgItem(IDC_RICHEDIT21_TEXT))
				{
					m_timeAxis.setCtrlState(TRUE);
				}
			}
			break;
		default:
			break;
		}
	}
	else if (pMsg->message == WM_KEYUP)
	{
		switch (pMsg->wParam)
		{
		case VK_CONTROL:		// 
			{
				CWnd* pWnd = FromHandle(pMsg->hwnd);
				if (!pWnd->IsKindOf(RUNTIME_CLASS(CEdit))
					&& pWnd != GetDlgItem(IDC_RICHEDIT21_TEXT))
				{
					m_timeAxis.setCtrlState(FALSE);
				}
			}
			break;
		default:
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CsyrMakerDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CRect reTime;
	m_timeAxis.GetWindowRect(&reTime);
	ClientToScreen(&point);
	if (reTime.PtInRect(point))
		m_timeAxis.OnLButtonDblClk(nFlags, point);

	CDialog::OnLButtonDblClk(nFlags, point);
}

void CsyrMakerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect reTime;
	m_timeAxis.GetWindowRect(&reTime);
	ClientToScreen(&point);
	if (reTime.PtInRect(point))
		m_timeAxis.OnLButtonDown(nFlags, point);

	CDialog::OnLButtonDown(nFlags, point);
}

void CsyrMakerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	CRect reTime;
	m_timeAxis.GetWindowRect(&reTime);
	ClientToScreen(&point);
	if (reTime.PtInRect(point))
		m_timeAxis.OnLButtonUp(nFlags, point);

	CDialog::OnLButtonUp(nFlags, point);
}

void CsyrMakerDlg::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	CRect reTime;
	m_timeAxis.GetWindowRect(&reTime);
	ClientToScreen(&point);
	if (reTime.PtInRect(point))
		m_timeAxis.OnRButtonDblClk(nFlags, point);

	CDialog::OnRButtonDblClk(nFlags, point);
}

void CsyrMakerDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	CRect reTime;
	m_timeAxis.GetWindowRect(&reTime);
	ClientToScreen(&point);
	if (reTime.PtInRect(point))
		m_timeAxis.OnRButtonDown(nFlags, point);

	CDialog::OnRButtonDown(nFlags, point);
}

void CsyrMakerDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	CRect reTime;
	m_timeAxis.GetWindowRect(&reTime);
	ClientToScreen(&point);
	if (reTime.PtInRect(point))
		m_timeAxis.OnRButtonUp(nFlags, point);
	else
	{
		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_MENU_SYRMAKER));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		CWnd* pWndPopupOwner = this;

		while (pWndPopupOwner->GetStyle() & WS_CHILD)
			pWndPopupOwner = pWndPopupOwner->GetParent();

		//ScreenToClient(&point);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			pWndPopupOwner);
	}

	CDialog::OnRButtonUp(nFlags, point);
}

void CsyrMakerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect reTime;
	m_timeAxis.GetWindowRect(&reTime);
	ClientToScreen(&point);
	if (reTime.PtInRect(point))
		m_timeAxis.OnMouseMove(nFlags, point);

	CDialog::OnMouseMove(nFlags, point);
}

BOOL CsyrMakerDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CRect reTime;
	m_timeAxis.GetWindowRect(&reTime);

	if (reTime.PtInRect(pt))
		m_timeAxis.OnMouseWheel(nFlags, zDelta, pt);

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CsyrMakerDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == ID_TIMER_SYR)
	{
		m_timeAxis.getCurTime() += 110;

		BOOL bStop = setTime(m_timeAxis.getCurTime());
// 		int min = m_timeAxis.getCurTime()/60000;
// 		int sec = m_timeAxis.getCurTime()%60000 / 1000;
// 		CString tm;
// 		tm.Format(_T("%02d:%02d"), min, sec);
// 		m_cTime.SetWindowText(tm);
// 
// 		int t_pos = m_timeAxis.getCurTime()/* + m_syr.getOffset() + 500*/;
// 
// 		BOOL bStop = TRUE;
// 		int cn = m_syr.getYYerCount();
// 		for (int i=0; i<cn; i++)
// 		{
// 			int ret = m_syr.calcYYerLrc(i, t_pos);
// 			if (ret == 0)
// 				bStop = FALSE;
// 			else if (ret == 1)
// 			{
// 				bStop = FALSE;
// 				setNick(i, m_syr.getYYerCurLrc(i));
// 			}
// 		}

		if (bStop)
		{
			KillTimer(ID_TIMER_SYR);
			GetDlgItem(IDC_BTN_BEGIN)->SetWindowText(_T("开始"));
			m_timeAxis.getCurTime() = 0;
			m_syr.reset();
			m_cTime.SetWindowText(_T("00:00"));
			setNick();
		}

		m_timeAxis.Invalidate();
	}

	CDialog::OnTimer(nIDEvent);
}

void CsyrMakerDlg::adjustPos()
{
	int cn = m_syr.getYYerCount();
	if (cn < 1)
	{
		return;
	}

	CRect re, re1, re2;
	GetDlgItem(IDC_EDIT_BASE)->GetWindowRect(&re);
	ScreenToClient(&re);
	int dy = re.Height() + 3;

	GetWindowRect(&re);
	re.bottom += dy * (cn - iYYerNumOld);
	MoveWindow(&re);

	GetDlgItem(IDC_GROUP_EDIT)->GetWindowRect(&re);
	ScreenToClient(&re);
	re.bottom += dy * (cn - iYYerNumOld);
	GetDlgItem(IDC_GROUP_EDIT)->MoveWindow(&re);

	GetDlgItem(IDC_GROUP_TEXT)->GetWindowRect(&re);
	ScreenToClient(&re);
	re.bottom += dy * (cn - iYYerNumOld);
	GetDlgItem(IDC_GROUP_TEXT)->MoveWindow(&re);

	GetDlgItem(IDC_RICHEDIT21_TEXT)->GetWindowRect(&re);
	ScreenToClient(&re);
	re.bottom += dy * (cn - iYYerNumOld);
	GetDlgItem(IDC_RICHEDIT21_TEXT)->MoveWindow(&re);

	GetDlgItem(IDC_STATIC_EDIT_BASE)->GetWindowRect(&re);
	ScreenToClient(&re);
	GetDlgItem(IDC_EDIT_BASE)->GetWindowRect(&re1);
	ScreenToClient(&re1);
	GetDlgItem(IDC_BUTTON_BASE)->GetWindowRect(&re2);
	ScreenToClient(&re2);
	for (int i=1; i<cn; i++)
	{
		re .top += dy;	re .bottom += dy;
		re1.top += dy;	re1.bottom += dy;
		re2.top += dy;	re2.bottom += dy;
		GetDlgItem(IDC_STATIC_EDIT_BASE+i)->MoveWindow(&re);
		GetDlgItem(IDC_EDIT_BASE+i)->MoveWindow(&re1);
		GetDlgItem(IDC_BUTTON_BASE+i)->MoveWindow(&re2);
	}

	for (int i=IDC_BUTTON_NEW; i<=IDC_BUTTON_FILELIST; i++)
	{
		GetDlgItem(i)->GetWindowRect(&re);
		ScreenToClient(&re);
		re.top += dy * (cn - iYYerNumOld);	re.bottom += dy * (cn - iYYerNumOld);
		GetDlgItem(i)->MoveWindow(&re);
	}

	iYYerNumOld = cn;
	Invalidate();
}

void CsyrMakerDlg::freeCtrl()
{
	int cn = (int)m_vStaticEdit.size();
	for (int i=1; i<cn; i++)
	{
		delete m_vStaticEdit[i];
		delete m_vEdit[i];
		delete m_vButton[i];
	}
	m_vStaticEdit.erase(m_vStaticEdit.begin()+1, m_vStaticEdit.end());
	m_vEdit.erase(m_vEdit.begin()+1, m_vEdit.end());
	m_vButton.erase(m_vButton.begin()+1, m_vButton.end());

	// 	iYYerNumOld = 1;
}

void CsyrMakerDlg::addYYer(int num/* =1 */)
{
	int cn = (int)m_vStaticEdit.size();
	CRect re(0, 0, 0, 0);
	CString str;
	for (int i=0; i<num; i++)
	{
		CStatic* pStaticEdit = new CStatic;
		str.Format(_T("%d麦"), cn+i+1);
		pStaticEdit->Create(str, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, re, this, IDC_STATIC_EDIT_BASE+cn+i);
		pStaticEdit->SetFont(GetFont());
		m_vStaticEdit.push_back(pStaticEdit);

		ClrcEdit* pEdit = new ClrcEdit;
		pEdit->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_EX_STATICEDGE, re, this, IDC_EDIT_BASE+cn+i);
		pEdit->SetFont(GetFont());
		pEdit->SetLimitText(20);
		pEdit->setIndex((int)m_vEdit.size());
		m_vEdit.push_back(pEdit);

		CButton* pButton = new CButton;
		pButton->Create(_T("-"), WS_CHILD | WS_VISIBLE | WS_TABSTOP/* | WS_EX_STATICEDGE*/, re, this, IDC_BUTTON_BASE+cn+i);
		pButton->SetFont(GetFont());
		m_vButton.push_back(pButton);
	}
}

void CsyrMakerDlg::delYYer(int index)
{
	int cn = m_vEdit.size();
	if (index > cn-1)
		return;

	CString tmp;
	for (int i=index; i<cn-1; i++)
	{
		m_vEdit.at(i+1)->GetWindowText(tmp);
		m_vEdit.at(i)->SetWindowText(tmp);
	}

	delete m_vStaticEdit.back();
	m_vStaticEdit.pop_back();

	delete m_vEdit.back();
	m_vEdit.pop_back();

	delete m_vButton.back();
	m_vButton.pop_back();
}

void CsyrMakerDlg::resetYYer()
{
	stopSyr();
	freeCtrl();
	addYYer(m_syr.getYYerCount()-1);
	adjustPos();

	m_timeAxis.reset();
	Invalidate();
}

BOOL CsyrMakerDlg::setTime(int time)
{
	int min = time / 60000;
	int sec = time % 60000 / 1000;
	CString tm;
	tm.Format(_T("%02d:%02d"), min, sec);
	m_cTime.SetWindowText(tm);

	int t_pos = time/* + m_syr.getOffset() + 500*/;

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

	return bStop;
}

int CsyrMakerDlg::setNick(int idx, CString nick)
{
	if (nick == _T(""))
		return 0;

	GetDlgItem(IDC_EDIT_BASE + idx)->SetWindowText(nick);

	if (m_pYYListDlg != NULL)
		m_pYYListDlg->setNick(idx, nick);

	return 1;
}

int CsyrMakerDlg::setNick()
{
	int cn = m_syr.getYYerCount();
	for (int i=0; i<cn; i++)
	{
// 		setNick(i, _T(""));
		GetDlgItem(IDC_EDIT_BASE + i)->SetWindowText( _T(""));
	}

	if (m_pYYListDlg != NULL)
		m_pYYListDlg->setNick();

	return 1;
}

int CsyrMakerDlg::delStrBlank(CString& str)
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

void CsyrMakerDlg::updateYYerLrc(ClrcEdit* pEdit)
{
	CString tmp;
	pEdit->GetWindowText(tmp);
	if (m_syr.getYYerCount() < 1)
		return;
	m_syr.editTimeLrc(pEdit->m_idx, m_syr.getYYerCurIdx(pEdit->m_idx), m_syr.getYYerCurTime(pEdit->m_idx), tmp);
}

int CsyrMakerDlg::setSyrPath(CString path)
{
	m_syrPath = path;
	return 1;
}

int CsyrMakerDlg::openSyr()
{
// 	m_syrPath = path;

	if (m_syr.Open(m_syrPath) == 0)
		return 0;

	resetYYer();
	return 1;
}

int CsyrMakerDlg::openSyr(CString path)
{
	m_syrPath = path;
	if (m_syr.Open(m_syrPath) == 0)
		return 0;

	resetYYer();
	return 1;
}

int CsyrMakerDlg::startSyr()
{
	SetTimer(ID_TIMER_SYR, 100, NULL);
	GetDlgItem(IDC_BTN_BEGIN)->SetWindowText(_T("暂停"));
	return 1;
}

int CsyrMakerDlg::pauseSyr()
{
	KillTimer(ID_TIMER_SYR);
	GetDlgItem(IDC_BTN_BEGIN)->SetWindowText(_T("开始"));
	return 1;
}

int CsyrMakerDlg::stopSyr()
{
	KillTimer(ID_TIMER_SYR);
	m_syr.reset();
	m_timeAxis.getCurTime() = 0;

	GetDlgItem(IDC_BTN_BEGIN)->SetWindowText(_T("开始"));
	m_cTime.SetWindowText(_T("00:00"));

// 	setNick();
// 	Invalidate();

	return 1;
}

void CsyrMakerDlg::OnBnClickedButtonNew()
{
	CFileDialog fdlg(FALSE, _T("SYR"), 0, OFN_HIDEREADONLY,	_T("花式字幕文件(*.syr)|*.syr|"), 0);
	if (fdlg.DoModal() == IDOK)
	{
		m_syr.New(fdlg.GetPathName());
		resetYYer();
	}
}

void CsyrMakerDlg::OnBnClickedButtonLoad()
{
	CFileDialog fdlg(TRUE, _T("SYR"), 0, OFN_HIDEREADONLY,	_T("花式字幕文件(*.syr)|*.syr|"), 0);
	if (fdlg.DoModal() == IDOK)
	{
		m_syrPath = fdlg.GetPathName();
// 		openSyr();
		if (m_syr.Open(fdlg.GetPathName()) == 0)
			return;

		resetYYer();
	}
}

void CsyrMakerDlg::OnBnClickedButtonSave()
{
	if (m_syr.getPath() == _T(""))
	{
		CFileDialog fdlg(FALSE, _T("SYR"), 0, OFN_HIDEREADONLY,	_T("花式字幕文件(*.syr)|*.syr|"), 0);
		if (fdlg.DoModal() == IDOK)
		{
			m_syr.SaveAs(fdlg.GetPathName());
		}
	}
	else
		m_syr.Save();
}

void CsyrMakerDlg::OnBnClickedButtonSaveas()
{
	CFileDialog fdlg(FALSE, _T("SYR"), 0, OFN_HIDEREADONLY,	_T("花式字幕文件(*.syr)|*.syr|"), 0);
	if (fdlg.DoModal() == IDOK)
	{
		m_syr.SaveAs(fdlg.GetPathName());
	}
}

void CsyrMakerDlg::OnBnClickedBtnBegin()
{
	CString state;
	GetDlgItem(IDC_BTN_BEGIN)->GetWindowText(state);
	if (state == _T("暂停"))
	{
		KillTimer(ID_TIMER_SYR);
		GetDlgItem(IDC_BTN_BEGIN)->SetWindowText(_T("开始"));
	}
	else if (state == _T("开始"))
	{
		SetTimer(ID_TIMER_SYR, 100, NULL);
		GetDlgItem(IDC_BTN_BEGIN)->SetWindowText(_T("暂停"));
	}
}

void CsyrMakerDlg::OnBnClickedBtnStop()
{
	KillTimer(ID_TIMER_SYR);
	m_syr.reset();
	m_timeAxis.getCurTime() = 0;

	GetDlgItem(IDC_BTN_BEGIN)->SetWindowText(_T("开始"));
	m_cTime.SetWindowText(_T("00:00"));

	Invalidate();

	setNick();
}

void CsyrMakerDlg::OnBnClickedBtnForward()
{
	m_timeAxis.getCurTime() += 500;
}

void CsyrMakerDlg::OnBnClickedBtnBackward()
{
	m_timeAxis.getCurTime() -= 500;
}

void CsyrMakerDlg::OnBnClickedBtnStepforward()
{
	m_timeAxis.getCurTime() = m_syr.getNextTimePoint(m_timeAxis.getCurTime());
	setTime(m_timeAxis.getCurTime());
	m_timeAxis.Invalidate();
}

void CsyrMakerDlg::OnBnClickedBtnStepback()
{
	m_timeAxis.getCurTime() = m_syr.getPrevTimePoint(m_timeAxis.getCurTime());
	setTime(m_timeAxis.getCurTime());
	m_timeAxis.Invalidate();
}

void CsyrMakerDlg::OnBnClickedButtonAdd()
{
	int cn = m_syr.getYYerCount();
	if (cn < 1)
		m_syr.New();

	m_syr.addYYer();

	addYYer();
	adjustPos();
}

void CsyrMakerDlg::OnBnClickedButtonDel()
{
	int cn = m_syr.getYYerCount();
	if (cn < 2)
		return;

	m_syr.delYYer(cn - 1);

	delYYer(cn - 1);
	adjustPos();
}

void CsyrMakerDlg::OnBnClickedRangeDel(UINT id)
{
	int cn = m_syr.getYYerCount();
	if (cn < 2)
		return;

	int index = id - IDC_BUTTON_BASE;
	if (index < 0 || index > cn - 1)
		return;

	m_syr.delYYer(index);

	delYYer(index);
	adjustPos();
}

void CsyrMakerDlg::OnBnClickedButtonAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CsyrMakerDlg::OnTimeaxisCopy()
{
	CtimeCopyDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		int time = dlg.getTime();
		if (dlg.isCopyTo())
		{
			if (m_syr.copySelTimesTo(time))
				m_timeAxis.Invalidate();
		}
		else
		{
			if (m_syr.copySelTimesOffset(time))
				m_timeAxis.Invalidate();
		}
	}
}

void CsyrMakerDlg::OnBnClickedButtonYylist()
{
	if (m_pYYListDlg == NULL)
	{
		m_pYYListDlg = new CyyListDlg();
		m_pYYListDlg->Create(IDD_DIALOG_YYLIST, this);
	}

	if (m_pYYListDlg->IsWindowVisible())
	{
		m_pYYListDlg->ShowWindow(SW_HIDE);
	}
	else
	{
		m_pYYListDlg->ShowWindow(SW_SHOW);
	}
}


#include "lrc.h"
void CsyrMakerDlg::OnSyrmakerLoadlrctp()
{
	CFileDialog fdlg(TRUE, _T("LRC"), 0, OFN_HIDEREADONLY,	_T("歌词文件(*.lrc)|*.lrc|"), 0);
	if (fdlg.DoModal() == IDOK)
	{
		CString lrcPath = fdlg.GetPathName();

		int iRet = 0;
		LRC_INFO_T* lrc = GetLRCFile(lrcPath.GetBuffer(), &iRet);
		if (iRet != 0)
			return;

		for (int i=0; i<lrc->num_t; i++)
		{
			m_syr.addYYersTime(lrc->tlrc[i].time);
		}
		m_timeAxis.Invalidate();

		DESTROY_LRC(&lrc);
	}
}

void CsyrMakerDlg::OnBnClickedButtonFilelist()
{
	if (m_pFileListDlg == NULL)
	{
		m_pFileListDlg = new CfileListDlg();
		m_pFileListDlg->Create(IDD_DIALOG_FILELIST, this);
	}

	if (m_pFileListDlg->IsWindowVisible())
	{
		m_pFileListDlg->ShowWindow(SW_HIDE);
	}
	else
	{
		m_pFileListDlg->ShowWindow(SW_SHOW);
	}
}

void CsyrMakerDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
}

LRESULT CsyrMakerDlg::OnNcHitTest(CPoint point)
{
	CRect re;
	GetClientRect(&re);

	ScreenToClient(&point);
	if (point.y >= 0 && point.y <= re.Height())
		return HTCAPTION;
	else
		return CDialog::OnNcHitTest(point);
}

void CsyrMakerDlg::OnNcRButtonUp(UINT nHitTest, CPoint point)
{
	CRect re;
	GetClientRect(&re);
	ScreenToClient(&point);
	if (nHitTest==HTCAPTION && re.PtInRect(point))
		SendMessage(WM_RBUTTONUP, (WPARAM)m_hWnd, MAKELPARAM(point.x, point.y));
	else
		CDialog::OnNcRButtonUp(nHitTest, point);
}

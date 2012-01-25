// YYnickDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "syrPlayer.h"
#include "syrPlayerDlg.h"
#include "AboutDlg.h"
#include "tinyxml/tinyxml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CONFIG_FILE "syrPlayer.xml"

#define ID_TIMER_SYR 998
#define ID_TIMER_LRC 999
#define ID_TIMER_STOP 1000


// CsyrPlayerDlg 对话框

CsyrPlayerDlg::CsyrPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CsyrPlayerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//m_hCtrl = NULL;
	m_lrc = NULL;

	//m_iYYType = 1;
	//m_bUseSBox = FALSE;
	//m_strSBoxName = _T("");

	m_iTimePos = 0;
	m_iCurYY = 0;

	m_isAlign = TRUE;
	m_bUseKey = TRUE;
}

CsyrPlayerDlg::~CsyrPlayerDlg()
{
	saveConfig();
	if (!m_bUseKey)
		unregHotKey();
}

void CsyrPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_YY, m_elYY);
	DDX_Control(pDX, IDC_TREE_LRCCON, m_treeLrcCon);
	DDX_Control(pDX, IDC_STATIC_TIME, m_cTime);
	DDX_Control(pDX, IDC_SLIDER_TIME, m_sliderTime);
	DDX_Control(pDX, IDC_EDIT_FILTER, m_editFilter);
}

BEGIN_MESSAGE_MAP(CsyrPlayerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_WM_TIMER()
	ON_WM_HSCROLL()
	ON_MESSAGE(WM_HOTKEY,&CsyrPlayerDlg::OnHotKey)//快捷键消息映射手动加入
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_YY, &CsyrPlayerDlg::OnLvnItemchangedListYy)
	ON_BN_CLICKED(IDC_BUTTON_UP, &CsyrPlayerDlg::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, &CsyrPlayerDlg::OnBnClickedButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CsyrPlayerDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BTN_BEGIN, &CsyrPlayerDlg::OnBnClickedBtnBegin)
	ON_BN_CLICKED(IDC_BTN_STOP, &CsyrPlayerDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_FORWARD, &CsyrPlayerDlg::OnBnClickedBtnForward)
	ON_BN_CLICKED(IDC_BTN_BACKWARD, &CsyrPlayerDlg::OnBnClickedBtnBackward)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_LRCCON, &CsyrPlayerDlg::OnTvnSelchangedTreeLrccon)
	ON_NOTIFY(NM_CLICK, IDC_TREE_LRCCON, &CsyrPlayerDlg::OnNMClickTreeLrccon)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TREE_LRCCON, &CsyrPlayerDlg::OnNMCustomdrawTreeLrccon)
	ON_EN_CHANGE(IDC_EDIT_FILTER, &CsyrPlayerDlg::OnEnChangeEditFilter)
	ON_BN_CLICKED(IDC_CHECK_USEKEY, &CsyrPlayerDlg::OnBnClickedCheckUsekey)
	ON_BN_CLICKED(IDC_BUTTON_KEYSET, &CsyrPlayerDlg::OnBnClickedButtonKeyset)
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, &CsyrPlayerDlg::OnBnClickedButtonAbout)
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

	// 十字星
	m_pic.SubclassDlgItem(IDC_PIC,this);

	// 时间
	CFont font; 
	font.CreatePointFont(720,_T("Arial"));    //720为字体的大小
	m_cTime.SetFont(&font);
	m_sliderTime.SetRangeMin(0);
	m_sliderTime.EnableWindow(FALSE);

	// YY列表
	m_elYY.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//m_elYY.InsertColumn(0, _T("帐号"), LVCFMT_LEFT, 100);
	//m_elYY.InsertColumn(1, _T("麦序"), LVCFMT_LEFT, 50);
	//m_elYY.InsertColumn(2, _T("昵称"), LVCFMT_LEFT, 100);
	m_elYY.InsertColumn(0, _T("帐号"), CListCtrlEditable::CtrlType_Static, 0, FALSE, LVCFMT_LEFT, 100);
	m_elYY.InsertColumn(1, _T("麦序"), CListCtrlEditable::CtrlType_Edit, ES_NUMBER, TRUE, LVCFMT_LEFT, 50);
	m_elYY.InsertColumn(2, _T("昵称"), CListCtrlEditable::CtrlType_Edit, 0, TRUE, LVCFMT_LEFT, 100);

	// 昵称and前后缀
	if (readConfig())
	{
		int cn = (int)m_vFilter.size();
		if (cn > 0)
		{
			CString strFiler = m_vFilter[0];
			for (int i=1; i<cn; i++)
			{
				strFiler += _T(" ");
				strFiler += m_vFilter[i];
			}
			m_editFilter.SetWindowText(strFiler);
		}
	}
	else
	{
		// 快捷键
		m_key[0].bCtrl = FALSE;	m_key[0].bAlt = FALSE;	m_key[0].bShift = FALSE;	m_key[0].uCode = VK_F5;
		m_key[1].bCtrl = TRUE;	m_key[1].bAlt = FALSE;	m_key[1].bShift = FALSE;	m_key[1].uCode = 'P';
		m_key[2].bCtrl = TRUE;	m_key[2].bAlt = FALSE;	m_key[2].bShift = FALSE;	m_key[2].uCode = 'S';
		m_key[3].bCtrl = TRUE;	m_key[3].bAlt = FALSE;	m_key[3].bShift = FALSE;	m_key[3].uCode = VK_RIGHT;
		m_key[4].bCtrl = TRUE;	m_key[4].bAlt = FALSE;	m_key[4].bShift = FALSE;	m_key[4].uCode = VK_LEFT;
	}

	// 快捷键
	GetDlgItem(IDC_BUTTON_KEYSET)->EnableWindow(m_bUseKey);
	if (m_bUseKey)
		regHotKey();

	// 初始状态，开始禁用
	GetDlgItem(IDC_BTN_BEGIN)->EnableWindow(FALSE);

	((CButton*)GetDlgItem(IDC_CHECK_USEKEY))->SetCheck(m_bUseKey);

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

void CsyrPlayerDlg::regHotKey()
{
	int mode = 0;
	mode |= m_key[0].bCtrl  ? MOD_CONTROL : 0;
	mode |= m_key[0].bAlt   ? MOD_ALT     : 0;
	mode |= m_key[0].bShift ? MOD_SHIFT   : 0;
	if (!RegisterHotKey(GetSafeHwnd(), IDC_BUTTON_FRESH, mode, m_key[0].uCode))
	{
		MessageBox(_T("快捷键【刷新】冲突，请重新设置！"));
	}
	mode = 0;
	mode |= m_key[1].bCtrl  ? MOD_CONTROL : 0;
	mode |= m_key[1].bAlt   ? MOD_ALT     : 0;
	mode |= m_key[1].bShift ? MOD_SHIFT   : 0;
	if (!RegisterHotKey(GetSafeHwnd(), IDC_BTN_BEGIN, mode, m_key[1].uCode))
	{
		MessageBox(_T("快捷键【开始】冲突，请重新设置！"));
	}
	mode = 0;
	mode |= m_key[2].bCtrl  ? MOD_CONTROL : 0;
	mode |= m_key[2].bAlt   ? MOD_ALT     : 0;
	mode |= m_key[2].bShift ? MOD_SHIFT   : 0;
	if (!RegisterHotKey(GetSafeHwnd(), IDC_BTN_STOP, mode, m_key[2].uCode))
	{
		MessageBox(_T("快捷键【停止】冲突，请重新设置！"));
	}
	mode = 0;
	mode |= m_key[3].bCtrl  ? MOD_CONTROL : 0;
	mode |= m_key[3].bAlt   ? MOD_ALT     : 0;
	mode |= m_key[3].bShift ? MOD_SHIFT   : 0;
	if (!RegisterHotKey(GetSafeHwnd(), IDC_BTN_FORWARD, mode, m_key[3].uCode))
	{
		MessageBox(_T("快捷键【快进】冲突，请重新设置！"));
	}
	mode = 0;
	mode |= m_key[4].bCtrl  ? MOD_CONTROL : 0;
	mode |= m_key[4].bAlt   ? MOD_ALT     : 0;
	mode |= m_key[4].bShift ? MOD_SHIFT   : 0;
	if (!RegisterHotKey(GetSafeHwnd(), IDC_BTN_BACKWARD, mode, m_key[4].uCode))
	{
		MessageBox(_T("快捷键【快退】冲突，请重新设置！"));
	}
}

void CsyrPlayerDlg::unregHotKey()
{
	UnregisterHotKey(GetSafeHwnd(), IDC_BUTTON_FRESH);
	UnregisterHotKey(GetSafeHwnd(), IDC_BTN_BEGIN);
	UnregisterHotKey(GetSafeHwnd(), IDC_BTN_STOP);
	UnregisterHotKey(GetSafeHwnd(), IDC_BTN_FORWARD);
	UnregisterHotKey(GetSafeHwnd(), IDC_BTN_BACKWARD);
}

int CsyrPlayerDlg::changeHandle(HANDLE hWnd)
{
	//m_hCtrl = hWnd;

	TCHAR buf[1024]=_T("\0");
	::GetWindowText((HWND)hWnd, buf, 1024);
	CString strTitle = buf;

	::GetClassName((HWND)hWnd, buf, 1024);
	CString strClass = buf;
	//if (strClass != _T("QWidget"))
	//	return 0;

	SYYPos yyp;
	yyp.m_hWnd = (HWND)hWnd;
	::GetWindowRect((HWND)hWnd, &yyp.m_rect);
	m_sYYP.push_back(yyp);
	m_elYY.InsertItem(m_elYY.GetItemCount(), strTitle);

	return 1;
}

int CsyrPlayerDlg::modifyYY(int idx, int item, CString val)
{
	if (idx < 0 || idx >= (int)m_sYYP.size())
		return -1;

	SYYPos& yyp = m_sYYP[idx];
	if (item == 1) //麦序
	{
		int idx = _ttoi(val.GetBuffer());
		setYYtoMap(yyp, idx);
		yyp.m_idx = idx;
	}
	else if (item == 2) //昵称
	{
		yyp.m_sNick = val;
	}
	else
	{
		return -2;
	}
	return 0;
}

void CsyrPlayerDlg::setYYtoMap(SYYPos syp, int idx)
{
	for (map<int, map<HWND, SYYPos> >::iterator it1 = m_mmyy.begin(); it1 != m_mmyy.end(); ++it1)
	{
		map<HWND, SYYPos>::iterator it2 = it1->second.find(syp.m_hWnd);
		if (it2 != it1->second.end())
		{
			it1->second.erase(it2);
			break;
		}
	}
	m_mmyy[idx][syp.m_hWnd] = syp;
}

int CsyrPlayerDlg::setNick(int idx, CString nick)
{
// 	if (nick == _T(""))
// 		return 0;

 	int cn = (int)m_sYYP.size();
 	if (cn < 1)
 		return 0;

	CString str;
	delStrBlank(nick);
	if (nick.IsEmpty())
	{
		return 0;
	}

	// 过滤
	vector<CString>::iterator it = m_vFilter.begin();
	for (; it!= m_vFilter.end(); it++)
	{
		if (nick.Find(*it) != -1)
			return 0;
	}

 	if (m_iLrcType == 1)
 	{
 		//CString strPrefix, strPostfix;
 		//m_cPrefix.GetWindowText(strPrefix);
 		//m_cPostfix.GetWindowText(strPostfix);
 		//str = strPrefix;
 		if (m_isAlign && cn > 1)
 		{
 			int n = 20 - lstrlen(str) - lstrlen(nick);
 			if (n < 0)
 			{
 				;
 			}
 			else
 			{
 				CString tt(_T(''), m_iCurYY*n/(cn-1));//　//
 				str += tt;
 			}
 		}
 		str += nick;
 		//str += strPostfix;
 		m_iCurYY++;
 		m_iCurYY = m_iCurYY % cn;
 	}
 	else if (m_iLrcType == 2 || m_iLrcType == 3)
	{
		str = nick;
	}

	map<HWND, SYYPos>& myy = m_mmyy[idx];
	for (map<HWND, SYYPos>::iterator it = myy.begin(); it != myy.end(); ++it)
	{
		SYYPos& syp = it->second;
		// 复制过去
		{
			::SendMessage(syp.m_hWnd, WM_LBUTTONDOWN, 1, MAKELONG(70, syp.m_rect.bottom - syp.m_rect.top - 15));
			::SendMessage(syp.m_hWnd, WM_LBUTTONUP, 0, MAKELONG(70, syp.m_rect.bottom - syp.m_rect.top - 15));

			for (int i=0; i<str.GetLength(); ++i)
			{
				::SendMessage(syp.m_hWnd, WM_CHAR, str.GetAt(i), 0);
			}

			::SendMessage(syp.m_hWnd, WM_KEYDOWN, VK_RETURN, 0x001C0001);
			::SendMessage(syp.m_hWnd, WM_KEYUP, VK_RETURN, 0xC01C0001);
		}
	}

	return 1;
}

int CsyrPlayerDlg::setNick()
{
	vector<SYYPos>::iterator it = m_sYYP.begin();
	for (; it!=m_sYYP.end(); it++)
	{
		SYYPos& syp = *it;
		// 复制过去
		{
			::SendMessage(syp.m_hWnd, WM_LBUTTONDOWN, 1, MAKELONG(70, syp.m_rect.bottom - syp.m_rect.top - 15));
			::SendMessage(syp.m_hWnd, WM_LBUTTONUP, 0, MAKELONG(70, syp.m_rect.bottom - syp.m_rect.top - 15));

			for (int i=0; i<syp.m_sNick.GetLength(); ++i)
			{
				::SendMessage(syp.m_hWnd, WM_CHAR, syp.m_sNick.GetAt(i), 0);
			}

			::SendMessage(syp.m_hWnd, WM_KEYDOWN, VK_RETURN, 0x001C0001);
			::SendMessage(syp.m_hWnd, WM_KEYUP, VK_RETURN, 0xC01C0001);
		}
	}
	return 1;
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

bool CsyrPlayerDlg::readConfig()
{
	// 解析xml
	USES_CONVERSION;

	TiXmlDocument doc;
	if(!doc.LoadFile(CONFIG_FILE))
	{
		return false;
	}

	TiXmlElement* root = doc.FirstChildElement("Config");
	if (root == NULL)
	{
		return false;
	}

	TiXmlElement* filterElem = root->FirstChildElement("Filter");
	while (filterElem != NULL)
	{
		TiXmlElement* valElem = filterElem->FirstChildElement("value");
		while (valElem != NULL)
		{
			CString str = UTF8ToUnicode(valElem->FirstChild()->Value());
			m_vFilter.push_back(str);
			valElem = valElem->NextSiblingElement("value");
		}

		filterElem = filterElem->NextSiblingElement("Filter");
	}

	TiXmlElement* keyElem = root->FirstChildElement("Key");
	if (keyElem != NULL)
	{
		const char* puse = keyElem->Attribute("use");
		if (puse != NULL)
			m_bUseKey  = atoi(puse);

		TiXmlElement* freshElem = keyElem->FirstChildElement("Fresh");
		if (freshElem != NULL)
		{
			m_key[0].bCtrl  = atoi(freshElem->Attribute("ctrl"));
			m_key[0].bShift = atoi(freshElem->Attribute("shift"));
			m_key[0].bAlt   = atoi(freshElem->Attribute("alt"));
			m_key[0].uCode  = atoi(freshElem->Attribute("code"));
		}
		TiXmlElement* beginElem = keyElem->FirstChildElement("Begin");
		if (beginElem != NULL)
		{
			m_key[1].bCtrl  = atoi(beginElem->Attribute("ctrl"));
			m_key[1].bShift = atoi(beginElem->Attribute("shift"));
			m_key[1].bAlt   = atoi(beginElem->Attribute("alt"));
			m_key[1].uCode  = atoi(beginElem->Attribute("code"));
		}
		TiXmlElement* stopElem = keyElem->FirstChildElement("Stop");
		if (stopElem != NULL)
		{
			m_key[2].bCtrl  = atoi(stopElem->Attribute("ctrl"));
			m_key[2].bShift = atoi(stopElem->Attribute("shift"));
			m_key[2].bAlt   = atoi(stopElem->Attribute("alt"));
			m_key[2].uCode  = atoi(stopElem->Attribute("code"));
		}
		TiXmlElement* forwardElem = keyElem->FirstChildElement("Forward");
		if (forwardElem != NULL)
		{
			m_key[3].bCtrl  = atoi(forwardElem->Attribute("ctrl"));
			m_key[3].bShift = atoi(forwardElem->Attribute("shift"));
			m_key[3].bAlt   = atoi(forwardElem->Attribute("alt"));
			m_key[3].uCode  = atoi(forwardElem->Attribute("code"));
		}
		TiXmlElement* backwardElem = keyElem->FirstChildElement("Backward");
		if (backwardElem != NULL)
		{
			m_key[4].bCtrl  = atoi(backwardElem->Attribute("ctrl"));
			m_key[4].bShift = atoi(backwardElem->Attribute("shift"));
			m_key[4].bAlt   = atoi(backwardElem->Attribute("alt"));
			m_key[4].uCode  = atoi(backwardElem->Attribute("code"));
		}
	}

	return true;
}

bool CsyrPlayerDlg::saveConfig()
{
	USES_CONVERSION;

	TiXmlDocument doc;

	TiXmlDeclaration* declaration = new TiXmlDeclaration("1.0", "utf-8", "");
	doc.LinkEndChild(declaration);

	TiXmlElement* root = new TiXmlElement("Config");
	doc.LinkEndChild(root);

	TiXmlText* text = NULL;
	CString str;

	TiXmlElement* filterElem = new TiXmlElement("Filter");
	root->LinkEndChild(filterElem);
	vector<CString>::iterator itFilter = m_vFilter.begin();
	for (; itFilter!=m_vFilter.end(); itFilter++)
	{
		TiXmlElement* val = new TiXmlElement("value");
		text = new TiXmlText(UnicodeToUTF8((LPCTSTR)(*itFilter)));
		val->LinkEndChild(text);
		filterElem->LinkEndChild(val);
	}

	TiXmlElement* keyElem = new TiXmlElement("Key");
	root->LinkEndChild(keyElem);
	keyElem->SetAttribute("use", m_bUseKey);
	TiXmlElement* freshElem = new TiXmlElement("Fresh");
	keyElem->LinkEndChild(freshElem);
	freshElem->SetAttribute("ctrl", m_key[0].bCtrl);
	freshElem->SetAttribute("shift", m_key[0].bShift);
	freshElem->SetAttribute("alt", m_key[0].bAlt);
	freshElem->SetAttribute("code", m_key[0].uCode);
	TiXmlElement* beginElem = new TiXmlElement("Begin");
	keyElem->LinkEndChild(beginElem);
	beginElem->SetAttribute("ctrl", m_key[1].bCtrl);
	beginElem->SetAttribute("shift", m_key[1].bShift);
	beginElem->SetAttribute("alt", m_key[1].bAlt);
	beginElem->SetAttribute("code", m_key[1].uCode);
	TiXmlElement* stopElem = new TiXmlElement("Stop");
	keyElem->LinkEndChild(stopElem);
	stopElem->SetAttribute("ctrl", m_key[2].bCtrl);
	stopElem->SetAttribute("shift", m_key[2].bShift);
	stopElem->SetAttribute("alt", m_key[2].bAlt);
	stopElem->SetAttribute("code", m_key[2].uCode);
	TiXmlElement* forwardElem = new TiXmlElement("Forward");
	keyElem->LinkEndChild(forwardElem);
	forwardElem->SetAttribute("ctrl", m_key[3].bCtrl);
	forwardElem->SetAttribute("shift", m_key[3].bShift);
	forwardElem->SetAttribute("alt", m_key[3].bAlt);
	forwardElem->SetAttribute("code", m_key[3].uCode);
	TiXmlElement* backwardElem = new TiXmlElement("Backward");
	keyElem->LinkEndChild(backwardElem);
	backwardElem->SetAttribute("ctrl", m_key[4].bCtrl);
	backwardElem->SetAttribute("shift", m_key[4].bShift);
	backwardElem->SetAttribute("alt", m_key[4].bAlt);
	backwardElem->SetAttribute("code", m_key[4].uCode);

	return doc.SaveFile(CONFIG_FILE);
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

// 		MessageBox(_T("请点击列表中的yy号来查看其字幕！"));
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

//	return (j >= lstrlen(tmp));
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

void CsyrPlayerDlg::OnEnChangeEditFilter()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString strFilter;
	m_editFilter.GetWindowText(strFilter);
	m_vFilter.clear();
	LPTSTR tmp = strFilter.GetBuffer();
	TCHAR* p = tmp;
	TCHAR* pp = p;
	CString sf;
	while (*p)
	{
		if (*p == _T(' '))
		{
			sf = pp;
			sf = sf.Left(int(p - pp));
			m_vFilter.push_back(sf);
			pp = p+1;
		}
		p++;
	}
	if (p != pp)
	{
		sf = pp;
		sf = sf.Left(int(p - pp));
		m_vFilter.push_back(sf);
		pp = p+1;
	}
}

void CsyrPlayerDlg::OnBnClickedButtonUp()
{
	int cursel = m_elYY.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if(cursel < 1)
	{
		return;
	}

	CString name = m_elYY.GetItemText(cursel, 0);
	CString nick = m_elYY.GetItemText(cursel, 1);
	CString sign = m_elYY.GetItemText(cursel, 2);

	m_elYY.DeleteItem(cursel);
	m_elYY.InsertItem(cursel-1, name);
	m_elYY.SetItemText(cursel-1, 1, nick);
	m_elYY.SetItemText(cursel-1, 2, sign);
	m_elYY.SetItemState(cursel-1, LVIS_SELECTED, LVIS_SELECTED);

	SYYPos syy = m_sYYP[cursel];
	m_sYYP.erase(m_sYYP.begin() + cursel);
	m_sYYP.insert(m_sYYP.begin() + cursel - 1, syy);
}

void CsyrPlayerDlg::OnBnClickedButtonDown()
{
	int cursel = m_elYY.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if(cursel == -1 || cursel == m_elYY.GetItemCount()-1)
	{
		return;
	}

	CString name = m_elYY.GetItemText(cursel, 0);
	CString nick = m_elYY.GetItemText(cursel, 1);
	CString sign = m_elYY.GetItemText(cursel, 2);

	m_elYY.DeleteItem(cursel);
	m_elYY.InsertItem(cursel+1, name);
	m_elYY.SetItemText(cursel+1, 1, nick);
	m_elYY.SetItemText(cursel+1, 2, sign);
	m_elYY.SetItemState(cursel+1, LVIS_SELECTED, LVIS_SELECTED);

	SYYPos syy = m_sYYP[cursel];
	m_sYYP.erase(m_sYYP.begin() + cursel);
	m_sYYP.insert(m_sYYP.begin() + cursel + 1, syy);
}

void CsyrPlayerDlg::OnBnClickedButtonDel()
{
	int cursel = m_elYY.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if(cursel == -1)
	{
		return;
	}

	m_elYY.DeleteItem(cursel);

	m_sYYP.erase(m_sYYP.begin() + cursel);
}

void CsyrPlayerDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER_TIME)
	{
		m_iTimePos = m_sliderTime.GetPos();
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CsyrPlayerDlg::OnLvnItemchangedListYy(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_elYY.OnItemChanged(pNMHDR, pResult);

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (m_iLrcType == 3 && pNMLV-> uNewState != 0)
	{
		SYYPos& syp = m_sYYP[pNMLV-> iItem];
		initList(syp.m_idx);
	}

	*pResult = 0;
}

LRESULT CsyrPlayerDlg::OnHotKey(WPARAM wParam,LPARAM lParam) 
{
	if (!m_bUseKey)
		return 0;

	switch (wParam)
	{
	case IDC_BTN_BEGIN:
		OnBnClickedBtnBegin();
		break;
	case IDC_BTN_STOP:
		OnBnClickedBtnStop();
		break;
	case IDC_BTN_FORWARD:
		OnBnClickedBtnForward();
		break;
	case IDC_BTN_BACKWARD:
		OnBnClickedBtnBackward();
		break;
	default:
		break;
	}

	return 0; 
} 

void CsyrPlayerDlg::OnBnClickedCheckUsekey()
{
	m_bUseKey = ((CButton*)GetDlgItem(IDC_CHECK_USEKEY))->GetCheck();
	GetDlgItem(IDC_BUTTON_KEYSET)->EnableWindow(m_bUseKey);
	if (m_bUseKey)
		regHotKey();
	else
		unregHotKey();
}

void CsyrPlayerDlg::OnBnClickedButtonKeyset()
{
	CkeySetDlg dlg;
// 	dlg.setUseKey(m_bUseKey);
	dlg.setKeySet(m_key);
	if (dlg.DoModal() == IDOK)
	{
// 		m_bUseKey = dlg.getUseKey();
	}
}

void CsyrPlayerDlg::OnBnClickedButtonAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
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


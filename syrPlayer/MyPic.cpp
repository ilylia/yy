// MyPic.cpp : implementation file
//

#include "stdafx.h"
#include "MyPic.h"
#include "resource.h"
#include "syrPlayerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyPic
// extern HWND g_hWnd;
CMyPic::CMyPic()
{
}

CMyPic::~CMyPic()
{
}


BEGIN_MESSAGE_MAP(CMyPic, CStatic)
	//{{AFX_MSG_MAP(CMyPic)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPic message handlers

void CMyPic::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetCapture();
	HCURSOR hc=LoadCursor(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDC_CURSOR1));
	::SetCursor(hc);
	HICON hicon2=LoadIcon(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDI_ICON2));
	this->SetIcon(hicon2);
	//GetDlgItem(IDOK)->SetWindowText("OK");
	//FromHandle(g_hMe)->OnTimer(1);
//	FromHandle(g_hMe)->SetTimer(1,600,NULL);
	CStatic::OnLButtonDown(nFlags, point);
}

void CMyPic::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ReleaseCapture();
	HICON hicon1=LoadIcon(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDI_ICON1));
	this->SetIcon(hicon1);
	
	//g_hWnd=::WindowFromPoint(point);
	POINT pt;
	::GetCursorPos(&pt);
	HANDLE hWnd = ::WindowFromPoint(pt);

//	CWnd* pParent = GetParent();
	CsyrPlayerDlg* pDlg = (CsyrPlayerDlg*)GetParent();
	pDlg->changeHandle(hWnd);

// 	//因为::WindowFromPoint(pt)对不可用的窗口无效，所以，为了能选中
// 	//灰色按钮等不可用的窗口(以进行激活等操作)，添加下面这段代码
// 	//读者也可以做一下试验，将下面*******间的代码注释掉，将不能选中不可用窗口
// 
// 	//*******************
// 	
// 	HWND grayHwnd = ::GetWindow(g_hWnd, GW_CHILD);
// 	RECT tempRc;
// 	BOOL bFind=FALSE;
//     while (grayHwnd)
// 	{
//         ::GetWindowRect(grayHwnd, &tempRc);
// 		if(::PtInRect(&tempRc,pt))
// 		{
// 			bFind = TRUE;
// 			break;
// 		}
//         else
//             grayHwnd = ::GetWindow(grayHwnd, GW_HWNDNEXT);
//         
// 	}//while
//     if(bFind == TRUE)
//     {
// 		bFind= FALSE;
//         g_hWnd = grayHwnd;
//     }
// 	else
//         ;//Wnd=UnHwnd
// 	
// 	//**********************//
// 	((CPage0*)FromHandle(g_hPage0))->m_editHWND.SetWindowText(Display((int)g_hWnd));
// 	
// 	char strClass[200]="\0";
// 	::GetClassName(g_hWnd,strClass,200);
// 	((CPage0*)FromHandle(g_hPage0))->m_editCLASS.SetWindowText(strClass);
// 	((CPage2*)FromHandle(g_hPage2))->SetDlgItemText(IDC_EDITCLASSNAME,strClass);
// 	
// 	char strTitle[200]="\0";
// 	::GetWindowText(g_hWnd,strTitle,200);
// 	((CPage0*)FromHandle(g_hPage0))->m_editTITLE.SetWindowText(strTitle);
// 	
// 	
// 	long iWNDID=GetWindowLong(g_hWnd,GWL_ID);
// 	((CPage0*)FromHandle(g_hPage0))->m_editWNDID.SetWindowText(Display((int)iWNDID));
// 	
// 	unsigned long iPID=0;
// 	GetWindowThreadProcessId(g_hWnd,&iPID);
// 	((CPage0*)FromHandle(g_hPage0))->m_editPID.SetWindowText(Display((int)iPID));
// 	
// 	CString strPath;
// 	strPath=getProcPath(iPID);
// 	((CPage0*)FromHandle(g_hPage0))->m_editPATH.SetWindowText(strPath);
// 	
// 	
// 	RECT rc;
// 	::GetWindowRect(g_hWnd, &rc);      //获得窗口矩形
// 	CString strRect;
// 	strRect.Format("(%d,%d),(%d,%d) %dx%d",rc.left,rc.top,rc.right,rc.bottom,
// 		rc.right-rc.left,rc.bottom-rc.top);
// 	((CPage0*)FromHandle(g_hPage0))->m_editRECT.SetWindowText(strRect);
// 	
// 	
// 	
// 	
// 	
// 	FromHandle(g_hMe)->KillTimer(1);
// 	//CPage1 pg1=((CSpyXXDlg*)FromHandle(g_hMe))->m_page1;
// 	
// 	CListBox* pListStyle=(CListBox*)(((CPage1*)FromHandle(g_hPage1))->GetDlgItem(IDC_LIST_STYLE));
// 	CListBox* pListExStyle=(CListBox*)(((CPage1*)FromHandle(g_hPage1))->GetDlgItem(IDC_LIST_EX_STYLE));
// 	CCheckListBox* pListStatus=(CCheckListBox*)(((CPage4*)FromHandle(g_hPage4))->GetDlgItem(IDC_LISTSTATUS));
// 	CEdit* pEditStyle=(CEdit*)(((CPage1*)FromHandle(g_hPage1))->GetDlgItem(IDC_EDIT_STYLE));
// 	CEdit* pEditExStyle=(CEdit*)(((CPage1*)FromHandle(g_hPage1))->GetDlgItem(IDC_EDIT_EX_STYLE));
// 	
// 	
// 	long   style = GetWindowLong(g_hWnd, GWL_STYLE);
// 	long  styleEx=    GetWindowLong(g_hWnd, GWL_EXSTYLE);
// 	
// 	pEditStyle->SetWindowText(Display((int)style));
// 	pEditExStyle->SetWindowText(Display((int)styleEx));
// 	
//     pListStyle->ResetContent();    //清空样式列表框
// 	pListExStyle->ResetContent();     //清空扩展样式列表框
// 	for(int i=0;i<9;i++)
// 		pListStatus->SetCheck(i,0);      //清除窗口状态列表框中的所有勾选
// 	if (style & WS_BORDER)
//         pListStyle->AddString("WS_BORDER");
//     
//     
//     if( style & WS_CAPTION)
// 		pListStyle->AddString("WS_CAPTION");
// 	
// 	if( style & WS_CHILD)
// 		pListStyle->AddString("WS_CHILD");
// 	
// 	if( style & WS_CLIPCHILDREN )
// 		pListStyle->AddString("WS_CLIPCHILDREN");
// 	
// 	if( style & WS_CLIPSIBLINGS )
// 		pListStyle->AddString("WS_CLIPSIBLINGS");
// 	
// 	if( style & WS_DISABLED )
// 		pListStyle->AddString("WS_DISABLED");
// 	else  
// 		
// 		pListStatus->SetCheck(1,1);
// 	
// 	if(style & WS_DLGFRAME)
// 		pListStyle->AddString("WS_DLGFRAME");
// 	
// 	if( style & WS_GROUP)
// 		pListStyle->AddString("WS_GROUP");
// 	
// 	if( style &WS_HSCROLL )
// 		pListStyle->AddString( "WS_HSCROLL");
// 	
// 	if( style&  WS_MAXIMIZE )
// 	{pListStyle->AddString("WS_MAXIMIZE");
// 	
// 	pListStatus->SetCheck(4,1);
// 	}
// 	
// 	if( style&  WS_MAXIMIZEBOX)
// 		pListStyle->AddString( "WS_MAXIMIZEBOX");
// 	
// 	if( style&  WS_MINIMIZE )
// 	{
// 		pListStyle->AddString("WS_MINIMIZE");
// 		pListStatus->SetCheck(5,1);
// 	} 
// 	
// 	if( style & WS_MINIMIZEBOX)
// 		pListStyle->AddString("WS_MINIMIZEBOX");
// 	
// 	if( style&  WS_POPUP)
// 		pListStyle->AddString( "WS_POPUP");
// 	
// 	if( style& WS_SYSMENU )
// 		pListStyle->AddString("WS_SYSMENU");
// 	
// 	if( style& WS_TABSTOP)
// 		pListStyle->AddString( "WS_TABSTOP");
// 	
// 	if( style&  WS_THICKFRAME)
// 		pListStyle->AddString( "WS_THICKFRAME");
// 	
// 	if( style& WS_VISIBLE )
// 	{pListStyle->AddString( "WS_VISIBLE");
// 	pListStatus->SetCheck(0,1);
// 	}
// 	
// 	if( style&  WS_VSCROLL )
// 		pListStyle->AddString("WS_VSCROLL"       );
//     
// 	if( style&  ES_READONLY )
// 	{pListStyle->AddString("ES_READONLY");
// 	pListStatus->SetCheck(3,1);
// 	}
// 	
// 	
// 	//下面是扩展样式
// 	if( styleEx &  WS_EX_ACCEPTFILES )
// 		pListExStyle->AddString( "WS_EX_ACCEPTFILES"        );
// 	if( styleEx &  WS_EX_DLGMODALFRAME )
// 		pListExStyle->AddString( "WS_EX_DLGMODALFRAME");
// 	
// 	if( styleEx &  WS_EX_NOPARENTNOTIFY )
// 		pListExStyle->AddString( "WS_EX_NOPARENTNOTIFY"       );
// 	if( styleEx &  WS_EX_TOPMOST )
// 	{
// 		pListExStyle->AddString( "WS_EX_TOPMOST");
// 		pListStatus->SetCheck(2,1);
// 	}
// 	
// 	
// 	if( styleEx &  WS_EX_TRANSPARENT )
// 		pListExStyle->AddString( "WS_EX_TRANSPARENT");
// 	
// 	if( styleEx &  WS_EX_MDICHILD )
// 		pListExStyle->AddString( "WS_EX_MDICHILD");
// 	
// 	if( styleEx &  WS_EX_TOOLWINDOW )
// 		pListExStyle->AddString( "WS_EX_TOOLWINDOW");
// 	
// 	if( styleEx &  WS_EX_WINDOWEDGE )
// 		pListExStyle->AddString( "WS_EX_WINDOWEDGE");
// 	
// 	if( styleEx &  WS_EX_CLIENTEDGE )
// 		pListExStyle->AddString( "WS_EX_CLIENTEDGE");
// 	
// 	if( styleEx &  WS_EX_CONTEXTHELP )
// 		pListExStyle->AddString( "WS_EX_CONTEXTHELP");
// 	
// 	if( styleEx &  WS_EX_RIGHT )
// 		pListExStyle->AddString( "WS_EX_RIGHT"        );
// 	if( styleEx &  WS_EX_RTLREADING )
// 		pListExStyle->AddString( "WS_EX_RTLREADING");
// 	
// 	if( styleEx &  WS_EX_LEFTSCROLLBAR )
// 		pListExStyle->AddString( "WS_EX_LEFTSCROLLBAR");
// 	
// 	if( styleEx &  WS_EX_CONTROLPARENT )
// 		pListExStyle->AddString( "WS_EX_CONTROLPARENT");
// 	
// 	if( styleEx &  WS_EX_STATICEDGE )
// 		pListExStyle->AddString( "WS_EX_STATICEDGE");
// 	
// 	if( styleEx &  WS_EX_APPWINDOW )
// 		pListExStyle->AddString( "WS_EX_APPWINDOW");
// 	
// 	//填写CPage2
// 	int classstyle = GetClassLong(g_hWnd, GCL_STYLE);
// 	(FromHandle(g_hPage2))->SetDlgItemText(IDC_EDITCLASSVALUE,Display(classstyle));
// 	
// 	CListBox* pListClass=(CListBox*)(FromHandle(g_hPage2)->GetDlgItem(IDC_LISTCLASS));
// 	pListClass->ResetContent();
// 	if(classstyle &  CS_BYTEALIGNCLIENT )
//         pListClass->AddString( "CS_BYTEALIGNCLIENT");
//     
//     if(classstyle &  CS_BYTEALIGNWINDOW )
//         pListClass->AddString( "CS_BYTEALIGNWINDOW");
//     
//     if(classstyle &  CS_CLASSDC )
//         pListClass->AddString( "CS_CLASSDC");
//     
//     if(classstyle &  CS_DBLCLKS )
//         pListClass->AddString( "CS_DBLCLKS");
//     
//     // Was CS_GLOBALCLASS (has same value)
// 	if(classstyle & CS_GLOBALCLASS)  
// 		pListClass->AddString( "CS_GLOBALCLASS");//PUBLICCLASS )
//     
//     if(classstyle &  CS_HREDRAW )
//         pListClass->AddString( "CS_HREDRAW");
//     
//     if(classstyle &  CS_NOCLOSE )
//         pListClass->AddString( "CS_NOCLOSE");
//     
//     if(classstyle &  CS_OWNDC )
//         pListClass->AddString( "CS_OWNDC");
//     
//     if(classstyle &  CS_PARENTDC )
//         pListClass->AddString( "CS_PARENTDC");
//     
//     if(classstyle &  CS_SAVEBITS )
//         pListClass->AddString( "CS_SAVEBITS");
//     
//     if(classstyle &  CS_VREDRAW )
//         pListClass->AddString( "CS_VREDRAW");
// 	
// 	//填写窗口标签页
// 	CPage3* pPage3=(CPage3*)FromHandle(g_hPage3);
// 	
// 	
// 	
//     HWND tempHandle;
// 	char tempstr[255]="\0";
//     tempHandle = g_hWnd;   //本窗口句柄
//     pPage3->SetDlgItemText(IDC_MYHWND, Display((int)tempHandle));
// 	
//     //获取本窗口标题
// 	::SendMessage(tempHandle, WM_GETTEXT, 255, (LPARAM)tempstr);
//     pPage3->SetDlgItemText(IDC_MYTITLE, tempstr);
// 	
//     tempHandle = ::GetNextWindow(g_hWnd, GW_HWNDPREV);   //上一窗口
//     pPage3->SetDlgItemText(IDC_PREHWND, Display((int)tempHandle));
// 	
//     //获取上一窗口标题
// 	memset(tempstr,0,255);
// 	::SendMessage(tempHandle, WM_GETTEXT, 255, (LPARAM)tempstr);
//     pPage3->SetDlgItemText(IDC_PRETITLE, tempstr);
// 	
//     tempHandle = ::GetNextWindow(g_hWnd, GW_HWNDNEXT);   //下一窗口
//     pPage3->SetDlgItemText(IDC_NEXTHWND,Display((int)tempHandle));
// 	
//     memset(tempstr,0,255);   //获取下一窗口标题
//     ::SendMessage(tempHandle, WM_GETTEXT, 255, (LPARAM)tempstr);
//     pPage3->SetDlgItemText(IDC_NEXTTITLE, tempstr);
// 	
//     tempHandle = ::GetParent(g_hWnd);   //父窗口
//     pPage3->SetDlgItemText(IDC_PARENTHWND, Display((int)tempHandle));
//     memset(tempstr,0,255);
//     ::SendMessage(tempHandle, WM_GETTEXT, 255, (LPARAM)tempstr);
//     pPage3->SetDlgItemText(IDC_PARENTTITLE,tempstr);
// 	
//     tempHandle = ::GetWindow(g_hWnd, GW_CHILD);        //第一子窗口
//     pPage3->SetDlgItemText(IDC_CHILDHWND,Display((int)tempHandle));
//     memset(tempstr,-0,255);
//     ::SendMessage(tempHandle, WM_GETTEXT, 255, (LPARAM)tempstr);
//     pPage3->SetDlgItemText(IDC_CHILDTITLE,tempstr);
// 	
//     tempHandle = ::GetWindow(g_hWnd, GW_OWNER);     //所有者窗口
//     pPage3->SetDlgItemText(IDC_OWNERHWND,Display((int)tempHandle));
//     memset(tempstr,0,255);
//     ::SendMessage(tempHandle, WM_GETTEXT, 255, (LPARAM)tempstr);
//     pPage3->SetDlgItemText(IDC_OWNERTITLE, tempstr);
// 	/////////////end
// 	
	
	CStatic::OnLButtonUp(nFlags, point);
}

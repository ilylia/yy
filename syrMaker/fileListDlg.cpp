// fileListDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "syrMaker.h"
#include "fileListDlg.h"
#include "Config.h"
#include "syrMakerDlg.h"


// CfileListDlg 对话框

IMPLEMENT_DYNAMIC(CfileListDlg, CDialog)

CfileListDlg::CfileListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CfileListDlg::IDD, pParent)
{

}

CfileListDlg::~CfileListDlg()
{
}

void CfileListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_FILE, m_cList);
}


BEGIN_MESSAGE_MAP(CfileListDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PATH, &CfileListDlg::OnBnClickedButtonPath)
	ON_BN_CLICKED(IDC_BUTTON_PATHFRE, &CfileListDlg::OnBnClickedButtonPathfre)
	ON_EN_CHANGE(IDC_EDIT_FILE_SEARCH, &CfileListDlg::OnEnChangeEditFileSearch)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_FILE, &CfileListDlg::OnNMDblclkTreeFile)
END_MESSAGE_MAP()


// CfileListDlg 消息处理程序

BOOL CfileListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetDlgItemText(IDC_EDIT_PATH,config.m_syrPath);

	if (!config.m_syrPath.IsEmpty())
		OnBnClickedButtonPathfre();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CfileListDlg::OnBnClickedButtonPath()
{
	TCHAR szDir[MAX_PATH];
	BROWSEINFO bi;
	ITEMIDLIST *pidl;
	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szDir;
	bi.lpszTitle = _T("请选择目录");
	bi.ulFlags = BIF_STATUSTEXT | /*BIF_USENEWUI | */BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	pidl = SHBrowseForFolder(&bi);
	if(pidl == NULL)  return;
	if(SHGetPathFromIDList(pidl, szDir))
	{
		config.m_syrPath = szDir;
		if (szDir[_tcslen(szDir)]!='\\' || szDir[_tcslen(szDir)]!='/')
		{
			config.m_syrPath += "\\";
		}
		config.m_bEdit = TRUE;
		SetDlgItemText(IDC_EDIT_PATH,config.m_syrPath);
		OnBnClickedButtonPathfre();
	}
}

void CfileListDlg::OnBnClickedButtonPathfre()
{
	m_vFiles.clear();
	m_cList.DeleteAllItems();

	FileFind(config.m_syrPath);
}

void CfileListDlg::OnEnChangeEditFileSearch()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CfileListDlg::FileFind(LPCTSTR lpszPath)
{
	TCHAR szFind[MAX_PATH];
	lstrcpy(szFind, lpszPath);

	if (szFind[lstrlen(szFind) - 1] != '\\')
		lstrcat(szFind, TEXT("\\"));
	lstrcat(szFind, TEXT("*.syr")); // 找所有文件

	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile(szFind, &wfd);
	if (hFind == INVALID_HANDLE_VALUE) // 如果没有找到或查找失败
		return;

	do
	{
		if (wfd.cFileName[0] == '.')
			continue; // 过滤这两个目录
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			TCHAR szFile[MAX_PATH];
			wsprintf(szFile, TEXT("%s\\%s"), lpszPath, wfd.cFileName);
			FileFind(szFile); // 如果找到的是目录，则进入此目录进行递归
		}
		else
		{
			// 对文件进行操作
// 			CString path = lpszPath;
// 			path += wfd.cFileName;
			AddFile(lpszPath, wfd.cFileName);
		}
	} while (FindNextFile(hFind, &wfd));
	FindClose(hFind); // 关闭查找句柄
}

void CfileListDlg::AddFile(LPCTSTR path, LPCTSTR file)
{
	CString str = path;
	str += TEXT("\\");
	str += file;
	m_vFiles.push_back(str);

	int cn = m_cList.GetCount();
	HTREEITEM hItem = m_cList.InsertItem(file);
	m_cList.SetItemData(hItem, (DWORD_PTR)cn);
}

void CfileListDlg::OnNMDblclkTreeFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	if (pNMTreeView->itemNew.mask != 1)
		return;

// 	HTREEITEM hCurItem = pNMTreeView->itemNew.hItem;
	HTREEITEM hCurItem = m_cList.GetSelectedItem();
	int i = (int)m_cList.GetItemData(hCurItem);
	int cn = (int)m_vFiles.size();
	if (i >= cn)
		return;
	CString path = m_vFiles.at(i);

	CsyrMakerDlg* pdlg = (CsyrMakerDlg*)GetParent();
	pdlg->openSyr(path);

	*pResult = 0;
}

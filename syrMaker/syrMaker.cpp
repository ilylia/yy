// syrMaker.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "syrMaker.h"
#include "syrMakerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CsyrMakerApp

BEGIN_MESSAGE_MAP(CsyrMakerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CsyrMakerApp 构造

CsyrMakerApp::CsyrMakerApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CsyrMakerApp 对象

CsyrMakerApp theApp;


// CsyrMakerApp 初始化

BOOL CsyrMakerApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	AfxInitRichEdit2();

	RegisterFileAndProgram();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	//获取命令行传过来的参数：双击打开的文件的文件路径名称
	CString strFilePathName = cmdInfo.m_strFileName;

	CsyrMakerDlg dlg;
	m_pMainWnd = &dlg;
	dlg.setSyrPath(strFilePathName);
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此处放置处理何时用“确定”来关闭
		//  对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用“取消”来关闭
		//  对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

//////关键函数：实现写注册表的函数////////
BOOL CsyrMakerApp::SetRegistryValue(HKEY hOpenKey, LPCTSTR szKey, LPCTSTR szValue, LPCTSTR szData)
{
	// validate input
	if( !hOpenKey || !szKey || !szKey[0] ||
		!szValue || !szData )
	{
		::SetLastError(E_INVALIDARG);
		return FALSE;
	}
	BOOL  bRetVal = FALSE;
	DWORD dwDisposition;
	DWORD dwReserved = 0;
	HKEY   hTempKey = (HKEY)0;
	// length specifier is in bytes, and some TCHAR
	// are more than 1 byte each
	DWORD dwBufferLength = lstrlen(szData) * sizeof(TCHAR);
	// Open key of interest
	// Assume all access is okay and that all keys will be stored to file
	// Utilize the default security attributes
	if( ERROR_SUCCESS == ::RegCreateKeyEx(hOpenKey, szKey, dwReserved,
		(LPTSTR)0, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, 0,
		&hTempKey, &dwDisposition) )
	{
		// dwBufferLength must include size of terminating nul
		// character when using REG_SZ with RegSetValueEx function
		dwBufferLength += sizeof(TCHAR);

		if( ERROR_SUCCESS == ::RegSetValueEx(hTempKey, (LPTSTR)szValue,
			dwReserved, REG_SZ, (LPBYTE)szData, dwBufferLength) )
		{
			bRetVal = TRUE;
		}
	}
	// close opened key
	if( hTempKey ){
		::RegCloseKey(hTempKey);
	}
	return bRetVal;
}

BOOL CsyrMakerApp::RegSetExtension(CString ext, CString strDocClassName, CString strDocCom)
{
	if( ext.IsEmpty() )
	{
		return FALSE;
	}

	CString csKey = _T(".") + ext;
	SetRegistryValue(HKEY_CLASSES_ROOT, csKey, _T(""), strDocClassName);

	if( !strDocCom.IsEmpty() )
	{
		csKey += _T("\\shell\\open\\command");
		SetRegistryValue(HKEY_CLASSES_ROOT, csKey, _T(""), strDocCom);
	}

	return TRUE;
}

BOOL CsyrMakerApp::RegSetDocumentType(CString strDocClassName, CString strDocDesc, CString strDocIcon, CString strDocCom)
{
	if( strDocClassName.IsEmpty())
	{
		return FALSE;
	}

	CString csKey = strDocClassName;
	SetRegistryValue(HKEY_CLASSES_ROOT, csKey, _T(""), strDocDesc);

	// DefaultIcon
	if( !strDocIcon.IsEmpty() )
	{
		csKey  = strDocClassName;
		csKey += _T("\\DefaultIcon");
		SetRegistryValue(HKEY_CLASSES_ROOT, csKey, _T(""), strDocIcon);
	}

	// shell\open\command
	if( !strDocCom.IsEmpty() )
	{
		csKey  = strDocClassName;
		csKey += _T("\\shell\\open\\command");
		SetRegistryValue(HKEY_CLASSES_ROOT, csKey, _T(""), strDocCom);
	}

	return TRUE;
}

void CsyrMakerApp::RegisterFileAndProgram()
{
#define strExternsionLength 1
	LPCTSTR strExtension[] =
	{
		_T("syr")
	};
// 	CGCFileTypeAccess TheFTA;

	CString  ext;				//关联文件的后缀名，如"txt"、"doc"等
	CString  strDocCom;
// 	CString  m_csDocumentShellOpenCommand;
	CString  strDocClassName;	//注册表中文件夹类名
	CString  strDocIcon;		//关联文件的默认图标
	CString  strDocDesc;

	TCHAR     szProgPath[MAX_PATH * 2];
	::GetModuleFileName(NULL, szProgPath, sizeof(szProgPath)/sizeof(TCHAR));
	CString csTempText;
	for(int i = 0; i < strExternsionLength; ++i)
	{
		ext = strExtension[i];

		strDocCom = _T("\"");
		strDocCom += szProgPath;
		strDocCom += _T("\" \"%1\"");

		//设置注册表中文件类的别名，例如可以是程序名称:**.exe
		strDocClassName = _T("syrfile");

		// use first icon in program
		strDocIcon = szProgPath;
		strDocIcon += _T(",1");

		RegSetExtension(strExtension[i], strDocClassName, strDocCom);
		RegSetDocumentType(strDocClassName, strDocDesc, strDocIcon, strDocCom);
	}
}

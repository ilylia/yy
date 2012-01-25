// syrMaker.cpp : ����Ӧ�ó��������Ϊ��
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


// CsyrMakerApp ����

CsyrMakerApp::CsyrMakerApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CsyrMakerApp ����

CsyrMakerApp theApp;


// CsyrMakerApp ��ʼ��

BOOL CsyrMakerApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	AfxInitRichEdit2();

	RegisterFileAndProgram();
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	//��ȡ�����д������Ĳ�����˫���򿪵��ļ����ļ�·������
	CString strFilePathName = cmdInfo.m_strFileName;

	CsyrMakerDlg dlg;
	m_pMainWnd = &dlg;
	dlg.setSyrPath(strFilePathName);
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˴����ô����ʱ�á�ȷ�������ر�
		//  �Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//  �Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

//////�ؼ�������ʵ��дע���ĺ���////////
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

	CString  ext;				//�����ļ��ĺ�׺������"txt"��"doc"��
	CString  strDocCom;
// 	CString  m_csDocumentShellOpenCommand;
	CString  strDocClassName;	//ע������ļ�������
	CString  strDocIcon;		//�����ļ���Ĭ��ͼ��
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

		//����ע������ļ���ı�������������ǳ�������:**.exe
		strDocClassName = _T("syrfile");

		// use first icon in program
		strDocIcon = szProgPath;
		strDocIcon += _T(",1");

		RegSetExtension(strExtension[i], strDocClassName, strDocCom);
		RegSetDocumentType(strDocClassName, strDocDesc, strDocIcon, strDocCom);
	}
}

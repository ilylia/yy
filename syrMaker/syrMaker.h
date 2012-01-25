// syrMaker.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CsyrMakerApp:
// 有关此类的实现，请参阅 syrMaker.cpp
//

class CsyrMakerApp : public CWinApp
{
public:
	CsyrMakerApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现
protected:
	void RegisterFileAndProgram();
	BOOL SetRegistryValue(HKEY hOpenKey, LPCTSTR szKey, LPCTSTR szValue, LPCTSTR szData);
	BOOL RegSetExtension(CString ext, CString strDocClassName, CString strDocCom);
	BOOL RegSetDocumentType(CString strDocClassName, CString strDocDesc, CString strDocIcon, CString strDocCom);

	DECLARE_MESSAGE_MAP()

// protected:
// 	CString  m_csExtension;				//关联文件的后缀名，如"txt"、"doc"等
// 	CString  m_csShellOpenCommand;
// 	CString  m_csDocumentShellOpenCommand;
// 	CString  m_csDocumentClassName;		//注册表中文件夹类名
// 	CString  m_csDocumentDefaultIcon;	//关联文件的默认图标
// 	CString  m_csDocumentDescription;
};

extern CsyrMakerApp theApp;
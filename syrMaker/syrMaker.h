// syrMaker.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CsyrMakerApp:
// �йش����ʵ�֣������ syrMaker.cpp
//

class CsyrMakerApp : public CWinApp
{
public:
	CsyrMakerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��
protected:
	void RegisterFileAndProgram();
	BOOL SetRegistryValue(HKEY hOpenKey, LPCTSTR szKey, LPCTSTR szValue, LPCTSTR szData);
	BOOL RegSetExtension(CString ext, CString strDocClassName, CString strDocCom);
	BOOL RegSetDocumentType(CString strDocClassName, CString strDocDesc, CString strDocIcon, CString strDocCom);

	DECLARE_MESSAGE_MAP()

// protected:
// 	CString  m_csExtension;				//�����ļ��ĺ�׺������"txt"��"doc"��
// 	CString  m_csShellOpenCommand;
// 	CString  m_csDocumentShellOpenCommand;
// 	CString  m_csDocumentClassName;		//ע������ļ�������
// 	CString  m_csDocumentDefaultIcon;	//�����ļ���Ĭ��ͼ��
// 	CString  m_csDocumentDescription;
};

extern CsyrMakerApp theApp;
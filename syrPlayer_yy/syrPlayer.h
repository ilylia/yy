// YYnick.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CYYnickApp:
// �йش����ʵ�֣������ YYnick.cpp
//

class CYYnickApp : public CWinApp
{
public:
	CYYnickApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CYYnickApp theApp;
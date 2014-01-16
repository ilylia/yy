#include "StdAfx.h"
#include "YYApiMgr.h"

#include "yyinc/yy-api-client.h"
#include "yyinc/yy-api-ex-client.h"
#include "yyinc/yy-api-events.h"
#include "yyinc/yy-api-error-codes.h"
#include "yyinc/yy-api-get.h"

const int max_nick_length = 20;

static CYYApiMgr* s_pYYApiMgr = NULL;
yy::IYYEx* g_pYYExApi = NULL;

CYYApiMgr* CYYApiMgr::GetInstance()
{
	if(s_pYYApiMgr==NULL)
	{
		s_pYYApiMgr = new CYYApiMgr();
	}

	return s_pYYApiMgr;
}

void CYYApiMgr::DelInstance()
{
	delete s_pYYApiMgr;
	s_pYYApiMgr = NULL;
}

CYYApiMgr::CYYApiMgr(void)
{
}

CYYApiMgr::~CYYApiMgr(void)
{
	Exit();
}

// public functions
bool CYYApiMgr::Init()
{
	ASSERT(g_pYYExApi==NULL);

	unsigned int nError = yy_e_unknown;
	CString cmdline = ::GetCommandLine();
	int blankIdx = cmdline.Find(_T(' '));
	cmdline.Delete(0, blankIdx + 1);
	nError = GetYYAPIEx(&g_pYYExApi, cmdline.GetBuffer());
	if(g_pYYExApi == NULL)
	{
		// clear
		Exit();
		return false;
	}

	// TOCONFIRM:这个在用的时候必须AddRef!!!
	g_pYYExApi->AddRef();
	nError = g_pYYExApi->Initialize();
	if (nError != yy_success)
	{
		// clear
		Exit();
		return false;
	}

	return true;
}

void CYYApiMgr::Exit()
{
	// TOCONFIRM:顺序不能反
	if(g_pYYExApi!=NULL)
	{
		// TOCONFIRM: 调用Release之后，请勿再调用任何与Platform相关的代码
		g_pYYExApi->Release();
		g_pYYExApi = NULL;
	}
}

CString CYYApiMgr::GetUserNick()
{
	CString nick;

	if (g_pYYExApi == NULL)
	{
		return nick;
	}

	yy::IUserEx* pUser = g_pYYExApi->GetUser();
	if (pUser == NULL)
	{
		return nick;
	}

	yy::IUserInfo *pUserInfo = NULL;
	pUser->GetCurrnetUserInfo(&pUserInfo);
	if(pUserInfo == NULL)
	{
		return nick;
	}

	char buf[256+1] = {0};
	pUserInfo->GetName(buf, 256);
	nick = CA2T(buf, CP_UTF8);

	return nick;
}

BOOL CYYApiMgr::Rename(CString nick)
{
	if (g_pYYExApi == NULL)
	{
		return FALSE;
	}

	yy::IUserEx* pUser = g_pYYExApi->GetUser();
	if (pUser == NULL)
	{
		return FALSE;
	}

	if (nick.GetLength() > max_nick_length)
	{
		nick.Delete(max_nick_length, nick.GetLength()-max_nick_length);
	}

	char name[256] = {0};
	strcpy(name, CT2A(nick, CP_UTF8));
	unsigned int nError = pUser->Rename(name);
	if (nError != yy_success)
	{
		return FALSE;
	}

	return TRUE;
}

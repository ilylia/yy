#pragma once

#include <mshtml.h>
#include <atlbase.h>
#include <oleacc.h>
#include <comdef.h>
#include <dispex.h>
#include <map>
using std::map;

class CsyrPlayerDlg;

class CHtmlWndMgr
{
	struct SHtmlScriptData
	{
		CComPtr<IDispatchEx> pScriptEx;
		DISPID lSetName;
		DISPID lGetName;
		UINT imId;
		map<CString, VARIANT> userInfo;

		SHtmlScriptData() : pScriptEx(NULL), lSetName(0), lGetName(0), imId(0)
		{
		}
	};
private:
	CHtmlWndMgr(void);
	~CHtmlWndMgr(void);

public:
	static CHtmlWndMgr* GetInstance();
	static void DelInstance();

public:
	bool Init(CsyrPlayerDlg* pDlg);
	void Release();

	void Refresh();
	void OnDelYY(unsigned int unID);

public:
	BOOL GetUserInfo(unsigned int unID, map<CString, VARIANT>& userInfo);
	BOOL Rename(unsigned int unID, CString nick);

private:
	BOOL GetHtmlWnd(HWND hwndIE, SHtmlScriptData& stData);
	unsigned int AddYY(SHtmlScriptData& stData);
	BOOL DelYY(unsigned int unID);
	SHtmlScriptData* GetPtrByID(unsigned int unID);
	BOOL exeWithNoArg(CComPtr<IDispatchEx> pScEx, DISPID dispid, map<CString, VARIANT>& ret);

private:
	HINSTANCE m_hinst;
	LPFNOBJECTFROMLRESULT m_pfObjectFromLresult;
	UINT m_unMsg;

	map<unsigned int, SHtmlScriptData> m_mapYYid2ptr;

	CsyrPlayerDlg* m_pDlg;
};

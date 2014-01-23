#pragma once

#include <mshtml.h>
#include <atlbase.h>
#include <oleacc.h>
#include <comdef.h>
#include <dispex.h>
#include <map>
using std::map;

class CyyListDlg;

class CHtmlWndMgr
{
	struct SHtmlScriptData
	{
		HWND hWnd;
		CComPtr<IDispatchEx> pScriptEx;
		DISPID lSetName;
		DISPID lGetName;
		UINT imId;
		map<CString, VARIANT> userInfo;

		SHtmlScriptData() : hWnd(NULL), pScriptEx(NULL), lSetName(0), lGetName(0), imId(0)
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
	bool Init(CyyListDlg* pDlg);
	void Release();

	void Refresh();
	BOOL ToggleYYAppVisible();

	void OnDelYY(unsigned int imId);

public:
	BOOL GetUserInfo(unsigned int imId, map<CString, VARIANT>& userInfo);
	BOOL Rename(unsigned int imId, CString nick);

private:
	BOOL GetHtmlWnd(HWND hWndQW, HWND hwndIE, SHtmlScriptData& stData);
	unsigned int AddYY(SHtmlScriptData& stData);
	BOOL DelYY(unsigned int imId);
	SHtmlScriptData* GetPtrByID(unsigned int imId);
	BOOL exeWithNoArg(CComPtr<IDispatchEx> pScEx, DISPID dispid, map<CString, VARIANT>& ret);

private:
	CyyListDlg* m_pDlg;
	HINSTANCE m_hinst;
	LPFNOBJECTFROMLRESULT m_pfObjectFromLresult;
	UINT m_unMsg;

	map<unsigned int, SHtmlScriptData> m_mapYYid2ptr;
	BOOL m_bShowYYApp;
};

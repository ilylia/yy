#pragma once

#include <mshtml.h>
#include <atlbase.h>
#include <oleacc.h>
#include <comdef.h>
#include <map>
using std::map;

class CsyrPlayerDlg;

class CHtmlWndMgr
{
	struct SHtmlScriptData
	{
		CComPtr<IDispatch> pScript;
		DISPID lSetName;
		DISPID lGetName;

		SHtmlScriptData() : pScript(NULL), lSetName(NULL), lGetName(NULL)
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

public:
	CString GetUserNick(unsigned int unID);
	BOOL Rename(unsigned int unID, CString nick);

private:
	BOOL GetHtmlWnd(HWND hwndIE, SHtmlScriptData& stData);
	unsigned int AddYY(SHtmlScriptData& stData);
	BOOL DelYY(unsigned int unID);
	SHtmlScriptData* GetPtrByID(unsigned int unID);

private:
	HINSTANCE m_hinst;
	LPFNOBJECTFROMLRESULT m_pfObjectFromLresult;
	UINT m_unMsg;

	map<CComPtr<IDispatch>, unsigned int> m_mapYYptr2id;
	map<unsigned int, SHtmlScriptData> m_mapYYid2ptr;
	unsigned int m_unLastID;

	CsyrPlayerDlg* m_pDlg;
};

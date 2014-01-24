#pragma once

#include <mshtml.h>
#include <atlbase.h>
#include <oleacc.h>
#include <comdef.h>
#include <dispex.h>
#include <list>
#include <map>
using std::list;
using std::map;

class CsyrPlayerDlg;

//临界区锁类  
class CriSection
{
public:
	CriSection() { ::InitializeCriticalSection(&m_critclSection); }
	~CriSection() { ::DeleteCriticalSection(&m_critclSection); }

	virtual void Lock() const { ::EnterCriticalSection((LPCRITICAL_SECTION)&m_critclSection); }
	virtual void Unlock() const { ::LeaveCriticalSection((LPCRITICAL_SECTION)&m_critclSection); }

private:
	CRITICAL_SECTION m_critclSection;
};

class MySemaphore
{
public:
	MySemaphore(long lInitCount = 0, long lMaxCount = 100) { m_hSemaphore = CreateSemaphore(NULL, lInitCount, lMaxCount, NULL); }
	~MySemaphore() { CloseHandle(m_hSemaphore); }

	virtual void Wait() const { WaitForSingleObject(m_hSemaphore, INFINITE); }
	virtual void Signal(long lCount = 1) const { ReleaseSemaphore(m_hSemaphore, lCount, NULL); }

private:
	HANDLE m_hSemaphore;
};

class CHtmlScriptData
{
public:
	CHtmlScriptData() : hWnd(NULL), pScriptEx(NULL), pScStream(NULL), lSetName(0), lGetName(0), imId(0), hThread(NULL), bRunning(TRUE)
	{
	}

	~CHtmlScriptData()
	{
		Release();
		userInfo.clear();
		ltRename.clear();
	}

	void Release()
	{
		bRunning = FALSE;
		if (hThread != 0)
		{
			iSema.Signal();
			iDelSema.Wait();
			CloseHandle(hThread);
			hThread = NULL;
		}
	}

	void BeginThread();

private:
	static unsigned int __stdcall ThdProc( void* lpParameter );
	void DoProc();

public:
	HWND hWnd; //yy app 窗口句柄
	CComPtr<IDispatchEx> pScriptEx; //脚本操作接口 对应yy.user
	LPSTREAM pScStream;
	DISPID lSetName; //rename的DISPID
	DISPID lGetName; //getCurrentUserInfo的DISPID
	UINT imId; //yy号
	HANDLE hThread; //线程句柄
	map<CString, VARIANT> userInfo; //userinfo

	BOOL bRunning;
	CriSection iLock;
	MySemaphore iSema;
	MySemaphore iDelSema;
	list<CString> ltRename;
};

class CHtmlWndMgr
{
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
	BOOL ToggleYYAppVisible();

	void OnDelYY(unsigned int imId);

public:
	BOOL GetUserInfo(unsigned int imId, map<CString, VARIANT>& userInfo);
	BOOL Rename(unsigned int imId, CString nick);

private:
	BOOL GetHtmlWnd(HWND hWndQW, HWND hwndIE, CHtmlScriptData** stData);
	unsigned int AddYY(CHtmlScriptData* stData);
	BOOL DelYY(unsigned int imId);
	CHtmlScriptData* GetPtrByID(unsigned int imId);
	BOOL exeWithNoArg(CComPtr<IDispatchEx> pScEx, DISPID dispid, map<CString, VARIANT>& ret);

private:
	CsyrPlayerDlg* m_pDlg;
	HINSTANCE m_hinst;
	LPFNOBJECTFROMLRESULT m_pfObjectFromLresult;
	UINT m_unMsg;

	map<unsigned int, CHtmlScriptData*> m_mapYYid2ptr;
	BOOL m_bShowYYApp;
};

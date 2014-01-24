#include "StdAfx.h"
#include "HtmlWndMgr.h"
#include "syrPlayerDlg.h"
#include <vector>
using std::vector;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int max_nick_length = 20;


void CHtmlScriptData::BeginThread()
{
	CoMarshalInterThreadInterfaceInStream(IID_IDispatchEx, pScriptEx, &pScStream);
	hThread = (HANDLE)_beginthreadex(NULL, 0, ThdProc, this, 0, NULL);
}

unsigned int CHtmlScriptData::ThdProc( void* lpParameter )
{
	if (lpParameter == NULL)
	{
		return 0;
	}

	CHtmlScriptData* pScData = (CHtmlScriptData*)lpParameter;
	pScData->DoProc();

	return 1;
}

void CHtmlScriptData::DoProc()
{
	CoInitialize(NULL);

	// 反列集得到IDisPatch指针
	CComPtr<IDispatchEx> pScEx;
	CoGetInterfaceAndReleaseStream(pScStream, IID_IDispatchEx, (LPVOID *)&pScEx);
	pScStream = NULL;

	while(bRunning)
	{
		iSema.Wait();
		CString nick;
		iLock.Lock();
		if (ltRename.size() > 0)
		{
			nick = ltRename.back();
			ltRename.clear();
		}
		iLock.Unlock();
		if (nick.GetLength() == 0)
		{
			continue;
		}

		VARIANT arg;
		arg.vt = VT_BSTR;
		arg.bstrVal = nick.AllocSysString();
		DISPPARAMS dpArgs = {&arg, NULL, 1, 0};
		HRESULT hr = pScEx->InvokeEx(lSetName, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD,
			&dpArgs, NULL, NULL, NULL);
		::SysFreeString( arg.bstrVal );
		if (FAILED(hr))
		{
			//return FALSE;
		}
	}

	::CoUninitialize();

	iDelSema.Signal();
}


static CHtmlWndMgr* s_pYYApiMgr = NULL;

CHtmlWndMgr* CHtmlWndMgr::GetInstance()
{
	if(s_pYYApiMgr==NULL)
	{
		s_pYYApiMgr = new CHtmlWndMgr();
	}

	return s_pYYApiMgr;
}

void CHtmlWndMgr::DelInstance()
{
	delete s_pYYApiMgr;
	s_pYYApiMgr = NULL;
}

CHtmlWndMgr::CHtmlWndMgr(void)
	: m_pDlg(NULL)
	, m_hinst(NULL)
	, m_pfObjectFromLresult(NULL)
	, m_unMsg(0)
	, m_bShowYYApp(TRUE)
{
}

CHtmlWndMgr::~CHtmlWndMgr(void)
{
	Release();
}

// public functions
bool CHtmlWndMgr::Init(CsyrPlayerDlg* pDlg)
{
	CoInitialize(NULL);
	m_hinst = ::LoadLibrary(_T("OLEACC.DLL"));
	m_unMsg = ::RegisterWindowMessage(_T("WM_HTML_GETOBJECT"));
	m_pfObjectFromLresult = (LPFNOBJECTFROMLRESULT)::GetProcAddress(m_hinst, "ObjectFromLresult");
	if(m_pfObjectFromLresult == NULL)
	{
		Release();
		return false;
	}

	m_pDlg = pDlg;
	return true;
}

void CHtmlWndMgr::Release()
{
	for (map<unsigned int, CHtmlScriptData*>::iterator itr = m_mapYYid2ptr.begin();
		itr != m_mapYYid2ptr.end(); ++itr)
	{
		if (itr->second != NULL)
		{
			delete itr->second;
		}
	}
	m_mapYYid2ptr.clear();

	if (m_hinst != NULL)
	{
		::FreeLibrary(m_hinst);
	}
	CoUninitialize();
}

void CHtmlWndMgr::Refresh()
{
	map<unsigned int, CHtmlScriptData*> mapHtmlWnd;
	CString winClass = _T("QWidget");
	CString ieClass = _T("Internet Explorer_Server");
	HWND hWndQW = NULL;
	do
	{
		hWndQW = ::FindWindowEx(NULL, hWndQW, winClass, NULL);
		if (hWndQW == NULL)
		{
			break;
		}

		HWND hWndIE = NULL;
		HWND hIEParent = hWndQW;
		do
		{
			hWndIE = ::FindWindowEx(hIEParent, hWndIE, NULL, NULL);
			if (hWndIE == NULL)
			{
				hWndIE = hIEParent;
				hIEParent = ::GetParent(hIEParent);
				if (hWndIE == hWndQW)
				{
					break;
				}
				continue;
			}
			else
			{
				TCHAR buf[1024]=_T("\0");
				::GetClassName(hWndIE, buf, 1024);
				CString strClass = buf;
				if (strClass == ieClass)
				{
					CHtmlScriptData* pData = NULL;
					if (GetHtmlWnd(hWndQW, hWndIE, &pData))
					{
						mapHtmlWnd[pData->imId] = pData;
					}
					break;
				}
				hIEParent = hWndIE;
				hWndIE = NULL;
				continue;
			}
		} while (TRUE);
	} while (hWndQW != NULL);

	for (map<unsigned int, CHtmlScriptData*>::iterator itr = mapHtmlWnd.begin();
		itr != mapHtmlWnd.end(); ++itr)
	{
		//if (m_mapYYid2ptr.find(itr->second.imId) == m_mapYYid2ptr.end())
		//{
		//add
		AddYY(itr->second);
		//}
	}
	vector<unsigned int> vtNeedDel;
	for (map<unsigned int, CHtmlScriptData*>::iterator itr = m_mapYYid2ptr.begin();
		itr != m_mapYYid2ptr.end(); ++itr)
	{
		if (mapHtmlWnd.find(itr->first) == mapHtmlWnd.end())
		{
			//del
			vtNeedDel.push_back(itr->first);
		}
	}
	for (vector<unsigned int>::iterator itr = vtNeedDel.begin();
		itr != vtNeedDel.end(); ++itr)
	{
		DelYY(*itr);
	}
}

BOOL CHtmlWndMgr::ToggleYYAppVisible()
{
	m_bShowYYApp = !m_bShowYYApp;
	int nCmdShow = m_bShowYYApp ? SW_SHOW : SW_HIDE;
	for (map<unsigned int, CHtmlScriptData*>::iterator itr = m_mapYYid2ptr.begin();
		itr != m_mapYYid2ptr.end(); ++itr)
	{
		::ShowWindow(itr->second->hWnd, nCmdShow);
	}
	return m_bShowYYApp;
}

void CHtmlWndMgr::OnDelYY(unsigned int imId)
{
	map<unsigned int, CHtmlScriptData*>::iterator _find = m_mapYYid2ptr.find(imId);
	if (_find != m_mapYYid2ptr.end())
	{
		if (_find->second != NULL)
		{
			delete _find->second;
		}
		m_mapYYid2ptr.erase(_find);
	}
}

BOOL CHtmlWndMgr::GetUserInfo(unsigned int imId, map<CString, VARIANT>& userInfo)
{
	CHtmlScriptData* pScData = GetPtrByID(imId);
	if (pScData == NULL)
	{
		return FALSE;
	}

	userInfo = pScData->userInfo;

	return TRUE;
}

BOOL CHtmlWndMgr::Rename(unsigned int imId, CString nick)
{
	CHtmlScriptData* pScData = GetPtrByID(imId);
	if (pScData == NULL)
	{
		return FALSE;
	}

	if (nick.GetLength() > max_nick_length)
	{
		nick.Delete(max_nick_length, nick.GetLength()-max_nick_length);
	}
	pScData->iLock.Lock();
	pScData->ltRename.push_back(nick);
	pScData->iLock.Unlock();
	pScData->iSema.Signal();

	return TRUE;
}

BOOL CHtmlWndMgr::GetHtmlWnd(HWND hWndQW, HWND hwndIE, CHtmlScriptData** pData)
{
	if (m_pfObjectFromLresult == NULL)
	{
		return FALSE;
	}

	LRESULT lres;
	::SendMessageTimeout(hwndIE, m_unMsg, 0L, 0L, SMTO_ABORTIFHUNG, 1000, (DWORD*)&lres);

	CComPtr<IHTMLDocument2> pDoc2;
	HRESULT hres = (*m_pfObjectFromLresult)(lres, IID_IHTMLDocument2, 0, (void**)&pDoc2);
	if(FAILED(hres) || pDoc2 == NULL)
	{
		return FALSE;
	}

	CComPtr<IDispatch> pScript = NULL;
	hres = pDoc2->get_Script(&pScript);
	if (FAILED(hres) || pScript == NULL)
	{
		return FALSE;
	}

	CComPtr<IDispatchEx> pScEx = NULL;
	hres = pScript->QueryInterface(IID_IDispatchEx, (void**)&pScEx);
	if (FAILED(hres) || pScEx == NULL)
	{
		return FALSE;
	}

	CComBSTR bstrMember(_T("yy"));
	DISPID dispid_yy = 0;
	hres = pScEx->GetDispID(bstrMember, fdexNameCaseSensitive, &dispid_yy);
	if (FAILED(hres))
	{
		return FALSE;
	}

	VARIANT ret_yy;
	DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
	hres = pScEx->InvokeEx(dispid_yy, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET,
		&dispparamsNoArgs, &ret_yy, NULL, NULL);
	if (FAILED(hres))
	{
		return FALSE;
	}

	CComPtr<IDispatch> pScYY = ret_yy.pdispVal;
	if (pScYY == NULL)
	{
		return FALSE;
	}

	CComPtr<IDispatchEx> pScYYEx = NULL;
	hres = pScYY->QueryInterface(IID_IDispatchEx, (void**)&pScYYEx);
	if (FAILED(hres))
	{
		return FALSE;
	}

	bstrMember = _T("user");
	DISPID dispid_user = 0;
	hres = pScYYEx->GetDispID(bstrMember, fdexNameCaseSensitive, &dispid_user);
	if (FAILED(hres))
	{
		return FALSE;
	}

	VARIANT ret_user;
	hres = pScYYEx->InvokeEx(dispid_user, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET,
		&dispparamsNoArgs, &ret_user, NULL, NULL);
	if (FAILED(hres))
	{
		return FALSE;
	}

	CComPtr<IDispatch> pScUser = ret_user.pdispVal;
	if (pScUser == NULL)
	{
		return FALSE;
	}

	CComPtr<IDispatchEx> pScUserEx = NULL;
	hres = pScUser->QueryInterface(IID_IDispatchEx, (void**)&pScUserEx);
	if (FAILED(hres))
	{
		return FALSE;
	}

	bstrMember = _T("rename");
	DISPID dispidSet = 0;
	hres = pScUserEx->GetDispID(bstrMember, fdexNameCaseSensitive, &dispidSet);
	if (FAILED(hres))
	{
		return FALSE;
	}

	bstrMember = _T("getCurrentUserInfo");
	DISPID dispidGet = 0;
	hres = pScUserEx->GetDispID(bstrMember, fdexNameCaseSensitive, &dispidGet);
	if (FAILED(hres))
	{
		return FALSE;
	}

	map<CString, VARIANT> userInfo;
	if (!exeWithNoArg(pScUserEx, dispidGet, userInfo))
	{
		return FALSE;
	}

	*pData = new CHtmlScriptData();
	(*pData)->hWnd = hWndQW;
	(*pData)->pScriptEx = pScUserEx;
	(*pData)->lSetName = dispidSet;
	(*pData)->lGetName = dispidGet;
	(*pData)->imId = userInfo[_T("imId")].uintVal;
	(*pData)->userInfo = userInfo;

	return TRUE;
}

unsigned int CHtmlWndMgr::AddYY(CHtmlScriptData* pData)
{
	if (pData == NULL || pData->pScriptEx == NULL)
	{
		return 0;
	}

	if (m_mapYYid2ptr.find(pData->imId) == m_mapYYid2ptr.end())
	{
		m_mapYYid2ptr[pData->imId] = pData;
		m_pDlg->AddYY(pData->imId);

		//thread
		m_mapYYid2ptr[pData->imId]->BeginThread();
	}
	else
	{
		m_mapYYid2ptr[pData->imId]->pScriptEx = pData->pScriptEx;
		m_mapYYid2ptr[pData->imId]->lGetName = pData->lGetName;
		m_mapYYid2ptr[pData->imId]->lSetName = pData->lSetName;
		delete pData;
	}

	return pData->imId;
}

BOOL CHtmlWndMgr::DelYY(unsigned int imId)
{
	map<unsigned int, CHtmlScriptData*>::iterator _find = m_mapYYid2ptr.find(imId);
	if (_find == m_mapYYid2ptr.end())
	{
		return FALSE;
	}

	delete _find->second;

	m_mapYYid2ptr.erase(_find);
	m_pDlg->DelYY(imId);

	return TRUE;
}

CHtmlScriptData* CHtmlWndMgr::GetPtrByID(unsigned int imId)
{
	map<unsigned int, CHtmlScriptData*>::iterator _find = m_mapYYid2ptr.find(imId);
	if (_find == m_mapYYid2ptr.end())
	{
		return NULL;
	}
	return _find->second;
}

BOOL CHtmlWndMgr::exeWithNoArg(CComPtr<IDispatchEx> pScEx, DISPID dispid, map<CString, VARIANT>& result)
{
	try
	{
		VARIANT ret;
		DISPPARAMS dpNoArgs = {NULL, NULL, 0, 0};
		HRESULT hr = pScEx->InvokeEx(dispid, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD,
			&dpNoArgs, &ret, NULL, NULL);
		if (FAILED(hr))
		{
			return FALSE;
		}

		CComPtr<IDispatch> pRet = ret.pdispVal;
		if (pRet == NULL)
		{
			return FALSE;
		}

		CComPtr<IDispatchEx> pRetEx = NULL;
		hr = pRet->QueryInterface(IID_IDispatchEx, (void**)&pRetEx);
		if (FAILED(hr) || pRetEx == NULL)
		{
			return FALSE;
		}

		DISPID dispid = DISPID_STARTENUM;
		do
		{
			hr = pRetEx->GetNextDispID(fdexEnumAll, dispid, &dispid);
			if (FAILED(hr))
			{
				break;
			}
			BSTR bstrName = NULL;
			hr = pRetEx->GetMemberName(dispid, &bstrName);
			if (FAILED(hr))
			{
				break;
				//return FALSE;
			}

			VARIANT ret_val;
			hr = pRetEx->InvokeEx(dispid, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET,
				&dpNoArgs, &ret_val, NULL, NULL);
			if (FAILED(hr))
			{
				break;
				//return FALSE;
			}

			result[bstrName] = ret_val;
			SysFreeString(bstrName);
			bstrName = NULL;
		} while (hr == NOERROR);
	}
	//catch (CMemoryException* e)
	//{
	//	
	//}
	catch (CException* /*e*/)
	{
		//MessageBox
		//bad
		//DelYY(imId);
		return FALSE;
	}

	return TRUE;
}

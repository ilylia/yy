#include "StdAfx.h"
#include "HtmlWndMgr.h"
#include "syrPlayerDlg.h"
#include <vector>
using std::vector;

const int max_nick_length = 20;

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
	: m_hinst(NULL)
	, m_pfObjectFromLresult(NULL)
	, m_unMsg(0)
	, m_unLastID(0)
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
	if (m_hinst != NULL)
	{
		::FreeLibrary(m_hinst);
	}
	CoUninitialize();
}

void CHtmlWndMgr::Refresh()
{
	map<CComPtr<IDispatchEx>, SHtmlScriptData> mapHtmlWnd;
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
					SHtmlScriptData stData;
					if (GetHtmlWnd(hWndIE, stData))
					{
						mapHtmlWnd[stData.pScriptEx] = stData;
					}
					break;
				}
				hIEParent = hWndIE;
				hWndIE = NULL;
				continue;
			}
		} while (TRUE);
	} while (hWndQW != NULL);

	for (map<CComPtr<IDispatchEx>, SHtmlScriptData>::iterator itr = mapHtmlWnd.begin();
		itr != mapHtmlWnd.end(); ++itr)
	{
		if (m_mapYYptr2id.find(itr->first) == m_mapYYptr2id.end())
		{
			//add
			AddYY(itr->second);
		}
	}
	for (map<CComPtr<IDispatchEx>, unsigned int>::iterator itr = m_mapYYptr2id.begin();
		itr != m_mapYYptr2id.end(); ++itr)
	{
		if (mapHtmlWnd.find(itr->first) == mapHtmlWnd.end())
		{
			//del
			DelYY(itr->second);
		}
	}
}

CString CHtmlWndMgr::GetUserNick(unsigned int unID)
{
	CString nick;

	SHtmlScriptData* pScData = GetPtrByID(unID);
	if (pScData == NULL)
	{
		return nick;
	}

	try
	{
		VARIANT ret;
		DISPPARAMS dpNoArgs = {NULL, NULL, 0, 0};
		HRESULT hr = pScData->pScriptEx->InvokeEx(pScData->lGetName, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD,
			&dpNoArgs, &ret, NULL, NULL);
		if (FAILED(hr))
		{
			return nick;
		}

		CComPtr<IDispatch> pRet = ret.pdispVal;
		if (pRet == NULL)
		{
			return nick;
		}

		CComPtr<IDispatchEx> pRetEx = NULL;
		hr = pRet->QueryInterface(IID_IDispatchEx, (void**)&pRetEx);
		if (FAILED(hr) || pRetEx == NULL)
		{
			return nick;
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
				return nick;
			}
			int r = wcscmp(bstrName, OLESTR("name"));
			SysFreeString(bstrName);
			bstrName = NULL;
			if (r == 0)
			{
				break;
			}
		} while (hr == NOERROR);

		VARIANT ret_name;
		hr = pRetEx->InvokeEx(dispid, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET,
			&dpNoArgs, &ret_name, NULL, NULL);
		if (FAILED(hr))
		{
			return nick;
		}

		nick = ret_name.bstrVal;
	}
	//catch (CMemoryException* e)
	//{
	//	
	//}
	catch (CException* /*e*/)
	{
		//MessageBox
		//bad
		DelYY(unID);
		return nick;
	}

	return nick;
}

BOOL CHtmlWndMgr::Rename(unsigned int unID, CString nick)
{
	SHtmlScriptData* pScData = GetPtrByID(unID);
	if (pScData == NULL)
	{
		return FALSE;
	}

	try
	{
		VARIANT arg;
		arg.vt = VT_BSTR;
		arg.bstrVal = nick.AllocSysString();
		DISPPARAMS dpArgs = {&arg, NULL, 1, 0};
		HRESULT hr = pScData->pScriptEx->InvokeEx(pScData->lSetName, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD,
			&dpArgs, NULL, NULL, NULL);
		::SysFreeString( arg.bstrVal );
		if (FAILED(hr))
		{
			return FALSE;
		}
	}
	//catch (CMemoryException* e)
	//{
	//	
	//}
	catch (CException* /*e*/)
	{
		//MessageBox
		//bad
		DelYY(unID);
		return FALSE;
	}

	return TRUE;
}

BOOL CHtmlWndMgr::GetHtmlWnd(HWND hwndIE, SHtmlScriptData& stData)
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

	stData.pScriptEx = pScUserEx;
	stData.lSetName = dispidSet;
	stData.lGetName = dispidGet;

	return TRUE;
}

unsigned int CHtmlWndMgr::AddYY(SHtmlScriptData& stData)
{
	if (stData.pScriptEx == NULL)
	{
		return 0;
	}

	if (m_mapYYptr2id.find(stData.pScriptEx) == m_mapYYptr2id.end())
	{
		unsigned int unNewID = ++m_unLastID;
		m_mapYYptr2id[stData.pScriptEx] = unNewID;
		m_mapYYid2ptr[unNewID] = stData;

		m_pDlg->AddYY(unNewID);
	}

	return m_mapYYptr2id[stData.pScriptEx];
}

BOOL CHtmlWndMgr::DelYY(unsigned int unID)
{
	map<unsigned int, SHtmlScriptData>::iterator _find = m_mapYYid2ptr.find(unID);
	if (_find != m_mapYYid2ptr.end())
	{
		m_mapYYptr2id.erase(_find->second.pScriptEx);
		m_mapYYid2ptr.erase(_find);

		m_pDlg->DelYY(unID);
	}
	return TRUE;
}

CHtmlWndMgr::SHtmlScriptData* CHtmlWndMgr::GetPtrByID(unsigned int unID)
{
	map<unsigned int, SHtmlScriptData>::iterator _find = m_mapYYid2ptr.find(unID);
	if (_find == m_mapYYid2ptr.end())
	{
		return NULL;
	}
	return &_find->second;
}

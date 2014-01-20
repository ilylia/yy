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
	map<CComPtr<IDispatch>, SHtmlScriptData> mapHtmlWnd;
	CString winClass = _T("QWidget");
	CString winTitle = _T("花弄影字幕");
	CString ieClass = _T("Internet Explorer_Server");
	HWND hWndQW = NULL;
	do
	{
		hWndQW = ::FindWindowEx(NULL, hWndQW, winClass, winTitle);
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
						mapHtmlWnd[stData.pScript] = stData;
					}
					break;
				}
				hIEParent = hWndIE;
				hWndIE = NULL;
				continue;
			}
		} while (TRUE);
	} while (hWndQW != NULL);

	for (map<CComPtr<IDispatch>, SHtmlScriptData>::iterator itr = mapHtmlWnd.begin();
		itr != mapHtmlWnd.end(); ++itr)
	{
		if (m_mapYYptr2id.find(itr->first) == m_mapYYptr2id.end())
		{
			//add
			AddYY(itr->second);
		}
	}
	for (map<CComPtr<IDispatch>, unsigned int>::iterator itr = m_mapYYptr2id.begin();
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
		// invoke assuming no method parameters
		VARIANT ret;
		DISPPARAMS dpNoArgs = {NULL, NULL, 0, 0};
		HRESULT hr = pScData->pScript->Invoke(pScData->lGetName, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD,
			&dpNoArgs, &ret, NULL, NULL);
		if (SUCCEEDED(hr))
		{
			nick = ret.bstrVal;
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
		arg.bstrVal = nick.GetBuffer();
		// invoke assuming no method parameters
		DISPPARAMS dpNoArgs = {&arg, NULL, 1, 0};
		pScData->pScript->Invoke(pScData->lGetName, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD,
			&dpNoArgs, NULL, NULL, NULL);
		//VARIANT ret;
		//ret.vt = VT_EMPTY;
		//CString sScript = _T("setName(\"") + nick + _T("\")");
		//CString Type = _T("javascript");
		//BSTR bstr = sScript.AllocSysString();
		//BSTR btype = Type.AllocSysString ();
		////执行要调用的脚本函数。
		//pHtmlWnd->execScript(bstr, btype, &ret);
		//::SysFreeString(bstr);
		//::SysFreeString(btype);
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

	//LPOLESTR strMethod[2] = {_T("setName"), _T("getName")};
	//DISPID idMethod[2];
	//hres = pScript->GetIDsOfNames(IID_NULL, strMethod, 2, LOCALE_SYSTEM_DEFAULT, idMethod);
	//if (FAILED(hres))
	//{
	//	return FALSE;
	//}
	CComBSTR bstrMember(_T("setName"));
	DISPID dispid = 0;
	hres = pScript->GetIDsOfNames(IID_NULL, &bstrMember, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if (FAILED(hres))
	{
		return FALSE;
	}

	stData.pScript = pScript;
	stData.lSetName = dispid;
	stData.lGetName = dispid;

	return TRUE;
}

unsigned int CHtmlWndMgr::AddYY(SHtmlScriptData& stData)
{
	if (stData.pScript == NULL)
	{
		return 0;
	}

	if (m_mapYYptr2id.find(stData.pScript) == m_mapYYptr2id.end())
	{
		unsigned int unNewID = ++m_unLastID;
		m_mapYYptr2id[stData.pScript] = unNewID;
		m_mapYYid2ptr[unNewID] = stData;

		m_pDlg->AddYY(unNewID);
	}

	return m_mapYYptr2id[stData.pScript];
}

BOOL CHtmlWndMgr::DelYY(unsigned int unID)
{
	map<unsigned int, SHtmlScriptData>::iterator _find = m_mapYYid2ptr.find(unID);
	if (_find != m_mapYYid2ptr.end())
	{
		m_mapYYptr2id.erase(_find->second.pScript);
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

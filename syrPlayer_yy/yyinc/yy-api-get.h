//! IYY接口创建
/*! 接口说明
 * @file dw-client-api.h
 * @create date 2011-10-03
 * @modify data 2012-03-29
 * @author simon.lv eRay.Jiang
 * @version 1.0
 * @modify list
 * ............................................................................
 * ............................................................................
 * 显式创建yy::IYY接口，对外发布
 * 广州华多网络科技有限公司 版权所有 (c) 2005-2011 DuoWan.com [多玩游戏]

 ******************************************************************************
 * 详细接口文档参考open.yy.com
 *******************************************************************************/

#ifndef YY_PLATFORM_SDK_YY_API_GET_H
#define YY_PLATFORM_SDK_YY_API_GET_H

#include <tchar.h>
#include "yy-api-client.h"
#include "yy-api-ex-client.h"

typedef yy::IYY* (*IYYCREATEFUNC)(void);
//typedef int (*IYYQUERYINTERFACE)(void **interface, char *guid, unsigned int size);
#define YYPLATFORM_SDK_FUNC_NAME ("GetYYOpenInterface")
//#define YYPLATFORM_SDK_QUERY_NAME ("QueryInterface")
#define YYPLATFORM_SDK_DLL_NAME _T("yyplatformsdk.dll")
//#define YY1_GUID ("{3E10F70E-5FA1-40e0-A26F-0CA5D0EE93CA}")

#define YYPLATFORM_SDK_EX_FUNC_NAME ("GetOpenApiInterface")
typedef int (*IYYQUERYINTERFACE)(int type, const wchar_t *cmdline, void** api);

inline yy::IYY* GetYYApi(unsigned int& err)
{
	HMODULE hModule = ::LoadLibrary(YYPLATFORM_SDK_DLL_NAME);

	if (hModule==NULL)
	{
		err = ::GetLastError();
		return NULL;
	}

	IYYCREATEFUNC pFunc = (IYYCREATEFUNC)::GetProcAddress(hModule, YYPLATFORM_SDK_FUNC_NAME);

	if (pFunc==NULL)
	{
		err = ::GetLastError();
		return NULL;
	}

	err = 0;
	return pFunc();
}

/*

inline int GetYYApi(IYY1 **iyy1)
{
	HMODULE hModule = ::LoadLibrary(YYPLATFORM_SDK_DLL_NAME);

	if (hModule==NULL)
	{
		return ::GetLastError();
	}

	IYYQUERYINTERFACE pQueryInterface = (IYYQUERYINTERFACE)::GetProcAddress(hModule, YYPLATFORM_SDK_QUERY_NAME);

	if (pFunc==NULL)
	{
		err = ::GetLastError();

		return err;
	}

	return pQueryInterface((void **)iyy1, YY1_GUID, 40);
}
*/

inline int GetYYAPIEx(yy::IYYEx** api, const wchar_t * cmd_line)
{
	HMODULE hModule = ::LoadLibrary(YYPLATFORM_SDK_DLL_NAME);

	if (hModule==NULL)
	{
		return -1;
	}

	IYYQUERYINTERFACE pFunc = (IYYQUERYINTERFACE)::GetProcAddress(hModule, YYPLATFORM_SDK_EX_FUNC_NAME);

	if (pFunc!=NULL)
	{
		return pFunc(1, cmd_line, (void **)api);
	}

	return -1;
}
#endif

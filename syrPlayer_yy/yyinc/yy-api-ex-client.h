//! IYY SDK 同步接口
/*! 接口说明
 * @file dw-client-api1.h
 * @create date 2012-10-03
 * @modify data 2012-10-29
 * @author simon.lv
 * @version 1.8
 * @modify list
 * ............................................................................
 * ............................................................................
 * yy open platform client api, 基于异步的yyplatform 网络通信协议
 * 广州华多网络科技有限公司 版权所有 (c) 2005-2011 DuoWan.com [多玩游戏]

 ******************************************************************************
 * 详细接口文档参考open.yy.com
 *******************************************************************************/

#ifndef _DW_OPENPLATFORM_CLIENT_API1_H_
#define _DW_OPENPLATFORM_CLIENT_API1_H_

#include "yy-api-client.h"

namespace yy
{
	extern "C"
	{
		struct IUserEx;
		struct IChannelEx;

		struct IIMEx;
		typedef IMicListItem IIMListItem;
		
		//![interface] 多玩yy客户端接口
		//!客户端所有接口入口
		struct IYYEx : public IYYUnk
		{
			//!初始化接口
			/*!
			 * \return int, 0 success, 其他值错误，请查看错误代码表
			*/
			virtual int Initialize() = 0;

			//!获取sdk 版本信息
			/*!
			 * \return int, sdk 版本信息
			*/
			virtual int GetVersion() = 0;

			//!获取YY 版本信息,返回使用UTF-8编码的字符串
			/*!
			 * \param char *version, [in] 用来接收版本信息的缓冲区首地址
			 * \param unsigned int size, [in] 用来接收版本信息的缓冲区大小 (0, 80]
			 * \return int, 0 success, 其他值错误，请查看错误代码表
			*/
			virtual int GetYYVersion(char *version, unsigned int size) = 0;

			//!获取用户接口
			/*!
			* \return IUserEx*,success返回有效指针，error返回NULL
			 */
			virtual IUserEx * GetUser() = 0;

			//!获取频道接口
			/*!
			* \return IChannelEx*,success返回有效指针，error返回NULL
			*/
			virtual IChannelEx * GetChannel() = 0;

			//!获取IM接口
			/*!
			*\return IIM*, success返回有效指针，error返回NULL
			*/
			virtual IIM * GetIM() = 0;

			//!获取语音接口
			/*!
			* \return IAudio*, success返回有效指针，error返回NULL
			*/
			virtual IAudio * GetAudio() = 0;

			//!获取网络接口
			/*!
			* \return INet*, success返回有效指针，error返回NULL
			*/ 
			virtual INet * GetNet() = 0;

			//!获取云存储接口
			/*!
			* \return ICloudData*, success返回有效指针，error返回NULL
			*/
			virtual ICloud * GetCloud() = 0;

			//!获取安全接口
			/*!
			* \return ISecurity*, success返回有效指针，error返回NULL
			*/
			virtual ISecurity * GetSecurity() = 0;

			//!获取临时语音聊天接口
			/*!
			* \return ITempAudioSession *, success返回有效指针，error返回NULL
			*/
			virtual ITempAudioSession * GetTempAudioSession() = 0;

			//!获取应用互动接口
			/*!
			* \return IInteractive *, success返回有效指针，error返回NULL
			*/
			virtual IInteraction * GetInteraction() = 0;

			//!注册yy api 事件
			/*!
			 * \return int, 0 success, 其他值错误，请查看错误代码表
			*/
			virtual int RegisterYYEvent(IYYEvent * event) = 0;

			//!反注册 yy api 事件
			/*!
			 * \return int, 0 success, 其他值错误，请查看错误代码表
			*/
			virtual int UnRegisterYYEvent(IYYEvent * event) = 0;

			//!获取财务接口
			/*!
			* \return IFinance *, success返回有效指针，error返回NULL
			*/
			virtual IFinance * GetFinance() = 0;

			/*! [interface]IM扩展接口
			*/
			virtual IIMEx * GetIMEx() = 0;

			/*! [interface]视频直播接口
			*/
			virtual IVideo * GetVideo() = 0;
		};

		//! [interface] 用户接口
		//! 用户相关接口 入口接口，包括获取当前的用户的信息，根据uid获取频道内的某个用户的基本信息，以及用户信息改变的基本事件
		struct IUserEx
		{
			//!获取当前用户信息
			/*! 
			/* \param IUserInfo**, success有效指针,error为NULL
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int GetCurrnetUserInfo(IUserInfo ** info) = 0;

			//!根据uid获取当前用户信息
			/*!
			* \param unsigned int [in] 用户id
			* \return IUserInfo**, success有效指针,error为NULL
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int GetUserInfo(unsigned int uid, IUserInfo ** info) = 0;

			//!注册用户事件
			/*!
			* \param IUserEvent* [in] 有效的用户事件指针
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int RegisterUserEvent(IUserEvent *e) = 0;

			//!反注册 用户事件
			/*!
			* \param IUserEvent* [in] 有效的用户事件指针
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int UnRegisterUserEvent(IUserEvent *e) = 0;

            //!更改昵称
			/*!
			* \param const char *name[in] 昵称， UTF-8编码
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int Rename(const char *name) = 0;
		};

		//![interface] 频道接口
		//!获取频道相关接口的入口，其中包括频道信息，子频道信息，麦序接口，频道子窗口接口，频道应用消息接口，频道信息 
		struct IChannelEx
		{
			//!获取当前子频道信息
			/*!
			* \param IChannelInfo **, error NULL
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int GetCurrentSubChannelInfo(IChannelInfo ** info) = 0;

			//!获取大频道信息
			/*!
			* \param IChannelInfo **, error NULL
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int GetCurrentChannelInfo(IChannelInfo ** info) = 0;

			//!通过频道id获取频道信息
			/*!
			* \param unsigned int cid [in],频道或者子频道id
			* \param IChannelInfo **, error NULL
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int GetChannelInfo(unsigned int cid, IChannelInfo ** info) = 0;

			//!获取频道树接口
			/*!
			* \return IChannelNode *, error NULL
			*/
			virtual const IChannelNode * GetChannelTree() = 0;

			//!获取麦序接口
			/*!
			* \return IChannelMicList *,error NULL
			*/
			virtual IChannelMicList * GetMicList() = 0;

			//!获取右键菜单接口
			/*!
			* \return IChannelUserListPopMenu *, error NULL
			*/
			virtual IChannelUserListPopMenu *GetUserListPopMenu() = 0; 
			
			//!获取频道消息接口
			/*!
			* \return IChannelAppMsg *, error NULL
			*/
			virtual IChannelAppMsg * GetAppMsg() = 0;
			
			//!获取频道子窗口接口
			/*!
			* \return IChannelSubWindow *, error NULL
			*/
			virtual IChannelSubWindow * GetSubWin() = 0;

			//!获取频道 tab页接口
			/*!
			* \return IChannelTabPage *, 有效指针值 success, NULl error
			*/
			virtual IChannelTabPage *GetTabPage() = 0;
			
			//!获取频道用户控制器
			/*!
			* \return IChannelUserController *, 有效指针值 success, NULl error
			*/
			virtual IChannelUserController *GetUserController() = 0;
			
			//!获取接待频道接口
			/*!
			* \return IReceptionChannel *, 有效指针值 success, NULl error
			*/
			virtual IReceptionChannel *GetReceptionChannel() = 0;

			//!获取模板接口
			/*!
			* \return IChannelTemplate *, 有效指针值 success, NULL error;
			*/
			virtual IChannelTemplate *GetTemplate() = 0;
		
			//!注册频道事件
			/*!
			* \param IChannelEvent * e, [in] [not NULL]
			* \return int, 0 success, 其他值 error,参考错误代码表
			*/
			virtual int RegisterChannelEvent(IChannelEvent *e) = 0;

			//!反注册频道事件
			/*!
			* \param IChannelEvent * e [in] [not NULL]
			* \return int, 0 success,其他值 error，查看错误代码表
			*/
			virtual int UnRegisterChannelEvent(IChannelEvent *e) = 0;

			/*!获取频道用户列表接口
			* \return IChannelUserList *, 成功为有效指针，失败返回 NULL
			*/
			virtual IChannelUserList * GetUserList() = 0;

            /*!获取频道用户列表接口
			* \return IChannelChat *, 成功为有效指针，失败返回 NULL
			*/
			virtual IChannelChat * GetChannelChat() = 0;
		};

		struct IIMUserInfoEx 
		{
			//返回值是UID
			virtual unsigned int GetUid() = 0;

			//返回值是IMID
			virtual unsigned int GetIMid() = 0;

			//返回值是VIPLIEVEL，0表示不是会员
			virtual unsigned int GetVipLevel() = 0;

			//返回0表示男，返回1表示女，其它值为未确定
			virtual int GetGender() = 0;

			//返回在线状态
			virtual int GetStatus() = 0;

			//return 0 sucess, failed 查看错误代码
			virtual int GetNick(char * name, unsigned int& size) = 0;

			//return 0 sucess, failed 查看错误代码
			virtual int GetRemark(char * remark, unsigned int& size) = 0;

			//path max size
			//return 0 sucess, failed 查看错误代码
			virtual int GetLocalHeaderImage(char * path, unsigned int& size) = 0;

			//max size = 4096
			//return 0 sucess, failed 查看错误代码
			virtual int GetCustomHeaderImageUrl(char * url, unsigned int& size) = 0;
		};

		/*![interface] IM扩展接口(注意，此接口需要保持与原IIMInternal兼容)
		*/
		struct IIMEx
		{
			//!获取好友列表
			virtual int GetAllBuddyList(IIMListItem **head) = 0;

			//!后去具体的IM信息
			virtual int GetIMUserInfo(unsigned int uid, IIMUserInfoEx **info) = 0;

			//!
			virtual int RegisterIMInternalEvent(IIMExEvent * event) = 0;

			//!
			virtual int UnRegisterIMInternalEvent(IIMExEvent * event) = 0;
		};
	}
}

#endif //_DW_OPENPLATFORM_CLIENT_API1_H_

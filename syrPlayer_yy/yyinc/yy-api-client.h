//! IYY SDK 同步接口
/*! 接口说明
 * @file dw-client-api.h
 * @create date 2011-10-03
 * @modify data 2012-05-15
 * @author simon.lv
 * @version 1.0
 * @modify list
 * ............................................................................
 * ............................................................................
 * yy open platform client api, 基于异步的yyplatform 网络通信协议
 * 广州华多网络科技有限公司 版权所有 (c) 2005-2011 DuoWan.com [多玩游戏]

 ******************************************************************************
 * 详细接口文档参考open.yy.com
 *******************************************************************************/

#ifndef _DW_OPENPLATFORM_CLIENT_API_H_
#define _DW_OPENPLATFORM_CLIENT_API_H_

#include "yy-api-types.h"
#include "yy-api-events.h"

namespace yy
{
	extern "C"
	{
		struct IYY;

		struct IIM;
		struct IUser;
		struct IAudio;		

		struct IChannel;		
		struct IChannelAppMsg;
		struct IChannelTabPage;
		struct IChannelUserController;

		// 公开IChannelUserList
		struct IChannelUserList;
		typedef IMicListItem IUserItem;
		
		// 以下网络相关
		struct INet;
		struct ICloud;
		struct ISecurity;

		struct IInteraction;
		struct ITempAudioSession;

		struct IChannelTemplate;

		//财务接口
		struct IFinance;

		//![interface] 多玩yy客户端接口
		//!客户端所有接口入口
		struct IYY : public IYYUnk
		{
			//!获取用户接口
			/*!
			* \return IUser*, success返回有效指针，error返回NULL
			 */
			virtual IUser * GetUser() = 0;

			//!获取频道接口
			/*!
			* \return IChannel*, success返回有效指针，error返回NULL
			*/
			virtual IChannel * GetChannel() = 0;

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

			//!获取sdk 版本信息
			/*!
			* \return int, sdk 版本信息
			*/
			virtual int GetVersion() = 0;

			//!初始化接口
			/*!
			* \return int, 0 success, 其他值错误，请查看错误代码表
			*/
			virtual int Initialize() = 0;

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
		};

		//! [interface]安全认证接口
		//! 用户安全登录相关功能，注意和服务器sdk 配合使用
		struct ISecurity
		{
			//!获取当前用户安全认证动态令牌
			/*
			* \param char* strToken [in][out] 令牌字符地址
			* \param unsigned int &size [in][out] 用户id 字符串大小为 (0, 128]
			* \return int, 0 success, 其他值错误，请查看错误代码表
			*/
			virtual int GetToken(char* strToken, unsigned int &size) = 0;

			//!用户举报
			/*
			* \return int, 0 success, 其他值错误，请查看错误代码表
			*/			
			virtual int ReportAbuse() = 0;

			//!敏感词过滤函数
			/*
            * \param boo &pass [in][out] 
			* \return int, 0 success, 其他值错误，请查看错误代码表
			*/
            virtual int KeywordFilter( const char* words, bool& pass ) = 0;
		};

		//! [interface] 用户接口
		//! 用户相关接口 入口接口，包括获取当前的用户的信息，根据uid获取频道内的某个用户的基本信息，以及用户信息改变的基本事件
		struct IUser
		{
			//!获取当前用户信息
			/*! 
			/* \return IUserInfo*, success有效指针,error为NULL
			*/
			virtual IUserInfo * GetCurrnetUserInfo() = 0;

			//!根据uid获取当前用户信息
			/*!
			* \param unsigned int [in] 用户id
			* \return IUserInfo*, success有效指针,error为NULL
			*/
			virtual IUserInfo * GetUserInfo(unsigned int uid) = 0;

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
		};

		//![interface] 频道右键菜单接口
		//!频道用户列表右键菜单设置 和取消设置 和对应的菜单事件设置
		struct IChannelUserListPopMenu
		{
			//!设置频道用户右键菜单
			/*!
			* \param [in] char* 显示在右键菜单字符串
			* \param [in] int   右键菜单显示字符串长度 最大长度 40
			* \return int, 0 success ,其他 值error,参考错误代码表
			*/
			virtual int SetPopMenu(const char *str, 
				unsigned int size) = 0;

			//!取消设置频道用户右键菜单
			/*!
			* \return int, 0 success ,其他 值error,参考错误代码表
			*/
			virtual int UnSetPopMemu() = 0;

			//!注册频道右键菜单事件
			/*!
			* \param [in] IChannelUserPopMenuEvent* e,有效的右键菜单事件指针
			* \return int, 0 success ,其他 值error,参考错误代码表
			*/
			virtual int RegisterPopMenuEvent(IChannelUserPopMenuEvent * e) = 0;

			//!反注册频道右键菜单事件
			/*!
			* \param IChannelUserPopMenuEvent* [int] 有效的右键菜单事件指针
			* \return int, 0 success ,其他 值error,参考错误代码表
			*/
			virtual int UnRegisterPopMenuEvent(IChannelUserPopMenuEvent * e) = 0;
		};

		//![interface] 应用消息接口
		//!该消息在频道内的公告栏下方出现一栏可点击的连接的文字信息，支持发送到指定的子频道所有人，和指定用户 的 接口入库
		struct IChannelAppMsg
		{
			//!发送应用消息
			/*!
			* \param unsigned int sub_channel_id, 子频道id
			* \param const char* view_str [in],显示字符串
			* \param unsigned size [in], 显示字符串的大小 最大长度 [200]
			* \param unsigned int  [in], 显示字符串中的链接首位置
			* \param unsigned int link_end_pos [in], 显示字符创的链接结束位置 //!eg. view_str :"my app link" 链接 link字符,link_begin_pos: 7, link_end_pos:10
			* \param unsigned int cookie [in], 自定义数据
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int SendMsgToSubChannel(unsigned int sub_channel_id,
				const char *view_str,
				unsigned int link_begin_pos,
				unsigned int link_end_pos,
				unsigned int cookie) = 0;

			//!发送应用消息
			/*!
			* \const unsigned int *users, 用户数组首地址
			* \const unsigned int size, 用户数组数量 最大[100]
			* \param const char* view_str [in],显示字符串
			* \param unsigned size [in], 显示字符串的大小 最大长度 [200]
			* \param unsigned int  [in], 显示字符串中的链接首位置
			* \param unsigned int link_end_pos [in], 显示字符创的链接结束位置 //!eg. view_str :"my app link" 链接 link字符,link_begin_pos: 7, link_end_pos:10
			* \param unsigned int cookie [in], 自定义数据
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int SendMsgToUsers(const unsigned int *users, 
				unsigned int size,
				const char *view_str,
				unsigned int link_begin_pos,
				unsigned int link_end_pos,
				unsigned int cookie) = 0;

			//!注册频道应用消息事件
			/*!
			* \param [in] IChannelUserPopMenuEvent* e,有效的右键菜单事件指针
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int RegisterChannelAppLinkEvent(IChannelAppLinkEvent *e) = 0;

			//!反注册频道应用消息事件
			/*!
			* \param [in] IChannelUserPopMenuEvent* e,有效的右键菜单事件指针
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int UnRegisterChannelAppLinkEvent(IChannelAppLinkEvent *e) = 0;

			//!创建新应用消息操作类
			/*!
			* \param [in] unsigned int cookie,消息 cookie
			* \return IAppMsg *, 有效指针 sucess, NULL  失败
			*/			
			virtual IAppMsg * CreateAppMsg(unsigned int cookie) = 0;

			//!发送新应用消息到指定的子频道
			/*!
			* \param [in] unsigned int sub_channel_id, 子频道ID
			* \param [in] IAppMsg *, 新应用消息指针
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/	
			virtual int SendMsgToSubChannelEx(unsigned int sub_channel_id, IAppMsg *msg) = 0;

			//!发送新应用消息到指定的用户
			/*!
			* \param [in] const unsigned int *users, 用户数组首地址
			* \param [in] unsigned int size, 数组大小
			* \param [in] IAppMsg *, 新应用消息指针
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/	
			virtual int SendMsgToUsersEx(const unsigned int *users, unsigned int size, IAppMsg *msg) = 0;
		};

		//![interface] 麦序接口
		//!获取麦序列表，注册麦序变化事件相关。注意，此接口只能在麦序模式下使用
		struct IChannelMicList
		{
			//!获取麦序列表
			/*!
			* \return IMicListItem * ,有效指针 success，error返回NULL
			*/
			virtual const IMicListItem * GetMicList() = 0;

			//!注册频道麦序事件
			/*!
			* \param [in] IMicListEvent* e,有效的右键菜单事件指针
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int RegisterMicListEvent(IMicListEvent *e) = 0;

			//!反注册频道麦序事件
			/*!
			* \param [in] IMicListEvent* e,有效的右键菜单事件指针
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int UnRegisterMicListEvent(IMicListEvent *e) = 0;

			//!加入麦序
			/*!
			* \return int, 0	success,其他值 error，查看错误代码表
			*/
			virtual int JoinMicList() = 0;

			//!离开麦序
			/*!
			* \return int, 0	success,其他值 error，查看错误代码表
			*/
			virtual int LeaveMicList() = 0;

			//!移动用户到麦序顶端
			/*!
			* \param unsigned int uid 用户id
			* \return int, 0	success,其他值 error，查看错误代码表
			*/
			virtual int MoveUserToTop(unsigned int uid) = 0;

			//!拉人上麦序
			/*!
			* \param unsigned int uid 用户id
			* \return int, 0	success,其他值 error，查看错误代码表
			*/			
			virtual int PullUserToMicList(unsigned int uid) = 0;

			//!踢麦序上的人
			/*!
			* \param unsigned int uid 用户id
			* \return int, 0	success,其他值 error，查看错误代码表
			*/			
			virtual int KickMicListUser(unsigned int uid) = 0;

			//!清空麦序
			/*!
			* \return int, 0	success,其他值 error，查看错误代码表
			*/			
			virtual int ClearMicList() = 0;

			//!获取连麦列表
			/*!
			* \param IMicListItem ** , 麦序列表头
			* \return int, 0	success,其他值 error，查看错误代码表
			*/
			virtual int GetLinkedMicList( const IMicListItem** h) = 0;

			//!设置某人跟 首号麦连麦，不在麦序上的 人不能设置连麦
			/*!
			* \param unsigned int uid, 麦序列表中的 用户 uid， 
			* \return int, 0	success,其他值 error，查看错误代码表
			*/
			virtual int LinkMicToTheQueueHead(unsigned int uid) = 0;

			//!将某人从连麦列表里去除
			/*!
			* \param unsigned int uid, 连麦列表中的 用户 uid， 
			* \return int, 0	success,其他值 error，查看错误代码表
			*/
			virtual int RemoveFromLinkedMicList(unsigned int uid) = 0;

			//!获取麦序模式
			/*!
			* \param EMicListModule &m, 麦序模式引用， 
			* \return int, 0	success,其他值 error，查看错误代码表
			*/
			virtual int GetMicListMode(YYEnums::EMicListMode &mode) = 0;

			//!设置麦序模式
			/*!
			* \param EMicListModule m, 麦序模式
			* \return int, 0	success,其他值 error，查看错误代码表
			*/
			virtual int SetMicListMode(YYEnums::EMicListMode m) = 0;

			//!注册频道麦序事件 扩展1
			/*!
			* \param [in] IMicListEvent2* e,有效的麦序事件指针
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/			
			virtual int RegisterMicListEvent2(IMicListEvent2 *e) = 0;

			//!反注册频道麦序事件 扩展1
			/*!
			* \param [in] IMicListEvent2* e,有效的麦序事件指针
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int UnRegisterMicListEvent2(IMicListEvent2 *e) = 0;

			//!将麦序上的某个人的位置往上提升一位，首麦和第二买不能上移
			/*!
			* \param unsigned int uid, [in] 麦上用户 uid
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int MoveUpOnePosition(unsigned int uid) = 0;

			//!将麦序上的某个人的位置往上下移一位，最后一位不能下移
			/*!
			* \param unsigned int uid, [in] 麦上用户 uid
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int MoveDownOnePosition(unsigned int uid) = 0;

			//!获取首位麦序秒数时长
			/*!
			* \param unsigned int &seconds, [in] 首麦秒数时长
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int GetFirstMicSeconds(unsigned int &seconds) = 0;

			//!双倍首麦时长秒数
			/*!
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int DoubleFirstMicSeconds() = 0;

			//!开麦
			/*!
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int EnableJoinMicList() = 0;

			//!禁麦
			/*!
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int DisableJoinMicList() = 0;

			//!是否开麦
			/*!
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
            virtual int IsJoinMicListEnabled( bool& enabled ) = 0;

			//!控麦
			/*!
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int ControlMic() = 0;

			//!放麦
			/*!
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int ReleaseMic() = 0;

			//!是否控麦状态
			/*!
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int IsMicListControlled( bool& controlled ) = 0;

			//!发送给二号麦麦序提醒
			/*!
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int SendMicListNotification() = 0;
			
			//!注册麦序管理事件
			/*!
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int RegisterMicListMgrEvent(IMicListMgrEvent *e) = 0;

			//!反注册麦序事件3
			/*!
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int UnRegisterMicListMgrEvent(IMicListMgrEvent *e) = 0;
		};

		//![interface] 频道子窗口 
		//!在频道的中间打开一个子窗口，用于应用自定义UI，在创建窗口成功后，可以接受窗口大小改变，和显示隐藏的事件
		struct IChannelSubWindow
		{
			//!创建频道内嵌窗口
			/*!
			* \param unsigned long &h [inout], Window HANDLE 引用
			* \param unsigned int &x [inout], 子窗口 x 位置
			* \param unsigned int &y [inout], 子窗口 y 位置
			* \param unsigned int &width [inout], 子窗口 宽
			* \param unsigned int &height [inout], 子窗口  高
			* \param bool &visible [inout], 是否显示
			* \return int, 0	success,其他值 error，查看错误代码表
			*/		
			virtual int CreateSubWindow(unsigned long &h, 
				unsigned int &x, 
				unsigned int &y, 
				unsigned int &width, 
				unsigned int &height,
				bool &visible) = 0;

			//!关闭内嵌窗口
			/*!
			* \return int, 0 success,其他值 error，查看错误代码表
			*/
			virtual int CloseSubWindow() = 0;

			//!注册频道事件
			/*!
			* \param ISubWindowEvent * e [in] [not NULL]
			* \return int, 0	success,其他值 error，查看错误代码表
			*/
			virtual int RegisterChannelSubWinEvent(ISubWindowEvent *e) = 0;

			//!反注册频道事件
			/*!
			* \param ISubWindowEvent * e [in] [not NULL]
			* \return int, 0	success,其他值 error，查看错误代码表
			*/
			virtual int UnRegisterChannelSubWinEvent(ISubWindowEvent *e) = 0;
		};

		//![interface] 频道用户控制接口
		//!频道用户发言控制，说话，跳转频道，获取用户所在子频道，等操作
		struct IChannelUserController
		{
			//!允许频道用户发言
			/*!
			* \param unsigned int uid [in] 用户id
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int EnableMsg(unsigned int uid) = 0;

			//!禁止频道用户发言
			/*!
			* \param unsigned int uid [in] 用户id
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int DisableMsg(unsigned int uid) = 0;

			//!允许频道用户说话
			/*!
			* \param unsigned int uid [in] 用户id
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int EnableSpeak(unsigned int uid) = 0;

			//!禁止频道用户说话
			/*!
			* \param unsigned int uid [in] 用户id
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int DisableSpeak(unsigned int uid) = 0;

			//!跳转频道
			/*!
			* \param unsigned int cid, [in] 子频道 ID
			* \return int, 0	success,其他值 error，查看错误代码表
			*/
			virtual int JumpToSubChannel(unsigned int cid) = 0;

			//!拉人进子频道
			/*!
			* \param unsigned int uid, [in] 用户 ID
			* \param unsigned int cid, [in] 子频道 ID
			* \return int, 0	success,其他值 error，查看错误代码表
			*/
			virtual int PullToSubChannel(unsigned int uid, unsigned int cid) = 0;	

			//!获取用户所在子频道ID
			/*!
			* \param unsigned int uid, [in] 用户 ID
			* \return int, 0	success,其他值 error，查看错误代码表
			*/
			virtual int GetUserSubChannelId(unsigned int uid, unsigned int &cid) = 0;	

			//!设置马甲
			/*!
			* \param unsigned int cid 频道id
			* \param unsigned int uid 用户id
			* \param IUserInfo::EUserRole role 用户角色(马甲)
			* \return int, 0	success,其他值 error，查看错误代码表
			*/
			virtual int SetUserRole(unsigned int cid, unsigned int uid, IUserInfo::EUserRole role) = 0;

            //!是否允许打字
			/*!
			* \param bool& enabled, [out] 是否允许
			* \return int, 0	success,其他值 error，查看错误代码表
			*/
            virtual int IsMsgEnabled( unsigned int uid, bool& enabled ) = 0;

            //!是否允许说话
			/*!
			* \param bool& enabled, [out] 是否允许
			* \return int, 0	success,其他值 error，查看错误代码表
			*/
            virtual int IsSpeakEnabled( unsigned int uid, bool& enabled ) = 0;
		};

		//![interface] 接待频道控制接口
		//! 接待频道控制设置和获取接口,接待频道只有一个
		struct IReceptionChannel
		{
			//!获取接待频道
			/*!
			* \param unsigned int &cid [inout] 子频道id
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int GetReceptionChannel(unsigned int &cid) = 0;

			//!设置接待频道
			/*!
			* \param unsigned int cid [in] 子频道id
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int SetReceptionChannel(unsigned int cid) = 0;

			//!反设置接待频道，移除接待频道
			/*!
			* \return int, 0 success ,其他值 error,参考错误代码表
			*/
			virtual int UnSetReceptionChannel() = 0;
		};

		//![interface] 频道接口
		//!获取频道相关接口的入口，其中包括频道信息，子频道信息，麦序接口，频道子窗口接口，频道应用消息接口，频道信息 
		struct IChannel
		{
			//!获取当前子频道信息
			/*!
			* \return IChannelInfo *, error NULL
			*/
			virtual IChannelInfo * GetCurrentSubChannelInfo() = 0;

			//!获取大频道信息
			/*!
			* \return IChannelInfo *, error NULL
			*/
			virtual IChannelInfo * GetCurrentChannelInfo() = 0;

			//!通过频道id获取频道信息
			/*!
			* \param unsigned int cid [in],频道或者子频道id
			* \return IChannelInfo *, error NULL
			*/
			virtual IChannelInfo * GetChannelInfo(unsigned int cid) = 0;

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

			//!注册频道事件
			/*!
			* \param IChannelEvent * e, [in] [not NULL]
			* \return int, 0 success, 其他值 error,参考错误代码表
			*/
			virtual int RegisterChannelEvent(IChannelEvent *e) = 0;

			//!反注册频道事件
			/*!
			* \param IChannelEvent * e [in] [not NULL]
			* \return int, 0	success,其他值 error，查看错误代码表
			*/
			virtual int UnRegisterChannelEvent(IChannelEvent *e) = 0;	

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

			//!获取频道模板接口
			/*!
			* \return IChannelTemplate *, 有效指针值 success, NULl error
			*/	
			virtual IChannelTemplate *GetTemplate() = 0;
		};

		//![interface] IM接口
		//!提供根据uid弹出聊天信息窗口功能，后续是马上提供是否是好友，增加好友的接口
		struct IIM
		{
			//!弹出IM对话框,只支持发送到当前好友
			/*!
			* \param int uid [in],用户唯一id
			* \param const char * [in], 聊天信息
			* \param unsigned int size [in], 信息长度 值范围(0, 40]
			* \return int, 0 success, 其他值 error,参考错误代码表
			*/
			virtual int ChatTo(unsigned int uid, const char *msg, unsigned int size) = 0;

			//!是否是好友
			/*!
			* \param int uid [in],用户唯一id
			* \return int, 0 是好友, 1 不是好友, 其他的值为其他错误信息
			*/
			virtual int IsFriend(unsigned int uid) = 0;

			//!增加好友
			/*!
			* \param int uid [in],用户唯一id
			* \return int, 0 success, 其他值 error,参考错误代码表
			*/
			virtual int AddFriend(unsigned int uid) = 0;

            //!打开某个频道群
			/*!
			* \param int cid [in],频道唯一id
			* \return int, 0 success, 其他值 error,参考错误代码表
			*/
			virtual int OpenChannelGroup(unsigned int cid) = 0;
		};

		//![interface] 语音接口 
		//!提供录音功能，和录音事件
		struct IAudio
		{
			//!开始录音
			/*!
			* \param const char *file_name [in],录音保存文件名
			* \return int, 0 success, 其他值 error,参考错误代码表
			*/
			virtual int StartRecord(const char *file_name) = 0;

			//!停止录音
			/*!
			* \return int, 0 success, 其他值 error,参考错误代码表
			*/
			virtual int StopRecord() = 0;

			//!注册音频设备更改回调
			/*!
			* \param IAudioEvent *e, [Not NULL]
			* \return int, 0 success 其他值 error 参考错误代码表
			*/
			virtual int RegisterAudioEvent(IAudioEvent *e) = 0;

			//!反注册IAudioDeviceChangedEvent
			/*!
			* \return int ,0 success，其他值 error 参考错误代码表
			*/
			virtual int UnRegisterAudioEvent(IAudioEvent *e) = 0;

			//!打开卡拉ok效果
			/*!
			* \return int ,0 success，其他值 error 参考错误代码表
			*/
			virtual int OpenKaraoke() = 0;

			//!关闭卡拉ok效果
			/*!
			* \return int ,0 success，其他值 error 参考错误代码表
			*/
			virtual int CloseKaraoke() = 0;

			//!开启混响
			/*!
			* \return int ,0 success，其他值 error 参考错误代码表
			*/
			virtual int EnableAudioMixing() = 0;

			//!关闭混响
			/*!
			* \return int ,0 success，其他值 error 参考错误代码表
			*/
			virtual int DisableAudioMixing() = 0;

			//!设置卡拉ok播放器路径
			/*!
			* \param const char *player_path [in], 播放器路径
			* \param char* origin_player_path [in][out], 前一次设置的播放器路径
			* \param char* origin_player_path_size [in][out], 前一次设置的播放器路径的长度
			* \param char* origin_software_mix_enable , 前一次是否开启了混音
			* \return int ,0 success，其他值 error 参考错误代码表
			*/
			virtual int SetKaraokePlayerPath(const char *player_path, 
				char *origin_player_path, unsigned int &origin_player_path_size,
				bool &origin_software_mix_enable) = 0;

			//!恢复卡拉ok播放器路径
			/*
			* \param const char *player_path [in], 要恢复的播放器路径
			* \param bool software_mix_enable [in]， 是否开启混音
			* \return int, 0 success, 其他值错误，请查看错误代码表
			*/
			virtual int ResetKaraokePlayerPath(const char *player_path, bool software_mix_enable) = 0;
		};

		//![interface]网络接口
		//!提供连接yy广播服务器，断开yy广播服务器，广播数据到当前频道所有的人，广播数据到指定的子频道，广播数据到指定的用户，和广播事件
		//!注意：只有当前的应用开启状态才能收到广播数据
		struct INet
		{
			//!连接到平台
			/*!
			* \return int, 0 success，其他值 error，参考错误代码表,在获取到IYY 接口之后必须在5秒之内连接上，否则连接会失效。
			*/
			virtual int Connect() = 0;

			//!主动断开
			/*!
			* \return int, 0 success，其他值 error，参考错误代码表
			*/
			virtual int Close() = 0;

			//!子频道数据广播
			/*!
			* \param unsigned int sub_channel_id [in], 子频道id,
			* \param const char *data [in], 广播数据流 开始地址
			* \param unsigned int size [in], 广播数据流大小， 值范围(0, 4096]
			* \return int, 0 success，其他值 error，参考错误代码表
			* \两次广播需要间隔20毫秒,否则广播数据可能会丢失。
			*/
			virtual int BroadcastToSubChannel(unsigned int sub_channel_id, const char *data, unsigned int size) = 0;

			//!全频道数据广播
			/*!
			* \param const char *data [in], 广播数据流 开始地址
			* \param unsigned int size [in], 广播数据流大小 值范围(0, 4096] 
			* \return int, 0 success，其他值 error，参考错误代码表
			* \两次广播需要间隔20毫秒,否则广播数据可能会丢失。
			*/
			virtual int BroadcastToAllChannel(const char *data, unsigned int size) = 0;

			//!广播到指定用户
			/*!
			* \param const char *data [in], 广播数据流 开始地址
			* \param unsigned int size [in], 广播数据流大小 值范围(0, 4096] 
			* \param int *u_array [in], 用户数组 开始地址
			* \param unsigned int count [in], 用户数组大小 值范围(0, 100] 
			* \return int, 0 success，其他值 error，参考错误代码表
			* \两次广播需要间隔20毫秒,否则广播数据可能会丢失。
			*/
			virtual int BroadcastToUsers(const char *data, unsigned int size, unsigned int *u_array, unsigned int count) = 0;

			//!注册网络回调
			/*!
			* \param INetEvent *e [in],网络事件
			* \return int, 0 注册success,其他值error,参考错误代码表
			*/
			virtual int RegisterNetEvent(INetEvent *e) = 0;

			//!反注册INetStateEvent 
			/*!
			* \return int, 0 success，其他值error 参考错误代码表
			*/
			virtual int UnRegisterNetEvent(INetEvent *e) = 0;
		};

		//![interface]云存储数据服务
		//!提供简单的数据存储，和操作能力，可以存储两个int，一个字符串，一个2进制流
		//!当前频道的数据只有橙马和紫马具备修改所有数据的能力，其他人一律只能修改自己的数据
		struct ICloud
		{
			//!增加数据
			/*!注意：操作频率 1秒钟一次
			/*!
			* \param ICloudData *data [in],云存储数据，必须设置值，否则无效
			* \return unsigned int, 0 success, 其他值 error, 参考错误代码表
			*/
			virtual int AddData(ICloudData *data, unsigned __int64  &key) = 0;

			//!修改数据
			/*!
			* \param ICloudData *data 云存储数据 ，必须设置值，否则无效
			* \param ICloudFilter *filter ,云存储过滤器，设置条件和条件值
			* \return int, 0 success, 其他值 error，参考错误代码表
			*/
			virtual int UpdateData(ICloudData *data, ICloudFilter *filter) = 0;

			//!删除数据
			/*!
			* \param ICloudFilter *filter [in],云存储过滤器，设置条件和条件值
			* \return int, 0 success, 其他值 error，参考错误代码表
			*/
			virtual int DeleteData(ICloudFilter *filter) = 0;

			//!查询数据
			/*!
			* \param ICloudDataList *filter [in],云存储过滤器，设置条件和条件值
			* \return ICloudDataList, 返回查询数据结构
			*/
			virtual ICloudDataList * QueryData(ICloudFilter *filter) = 0;

			//!创建云存储数据
			/*!
			* \return ICloudData *, success返回有效指针，error返回NULL
			*/
			virtual ICloudData * CreateCloudData() = 0;

			//!创建数据过滤器
			/*!
			* \return ICloudFilter *, success返回有效指针，error返回NULL
			*/
			virtual ICloudFilter * CreateCloudFilter() = 0;
		};

		/*![interface] 频道子频道用户列表
		*/
		struct IChannelUserList
		{
			//!当前子频道用户个数
			/*!
			* \param [in] unsigned int cid，子频道id
			* \param [inout] unsigned int *count，子频道人数
			* \return 0 成功, 失败为其他值，参考错误代码表
			*/
			virtual int GetUserCount(unsigned int cid, unsigned int * count) = 0;

			//!当前子频道用户列表，暂时开放top 200
			/*!
			* \param [in] unsigned int cid，子频道id
			* \param [in] usingned int index，指定序号
			* \param [inout] IUserItem ** list，用户列表，
			* \return 0 成功, 失败为其他值，参考错误代码表， 成功后，list 为有效的用户列表指针
			*/
			virtual int GetUserList(unsigned int cid, unsigned int index, IUserItem ** list) = 0;
		};

		//![interface]频道tab页接口
		//!提供创建一个频道tabpage 子窗口
		//!注意：只能创建一个tabpage
		struct IChannelTabPage
		{
			//!创建频道tab页窗口
			/*!
			* \param unsigned long &h [inout], Window HANDLE 引用
			* \param unsigned int &x [inout], 子窗口 x 位置
			* \param unsigned int &y [inout], 子窗口 y 位置
			* \param unsigned int &width [inout], 子窗口 宽
			* \param unsigned int &height [inout], 子窗口  高
			* \param bool &visible [inout], 是否显示
			* \return int, 0	success,其他值 error，查看错误代码表
			*/	
			virtual int CreateTabPage(unsigned long &h, 
				unsigned int &x, 
				unsigned int &y, 
				unsigned int &width, 
				unsigned int &height,
				bool &visible) = 0;

			//!关闭tabpage窗口
			/*!
			* \return int, 0 success,其他值 error，查看错误代码表
			*/
			virtual int CloseTabPage() = 0;

			//!注册tab页窗口事件
			/*!
			* \param ISubWindowEvent * e [in] [not NULL]
			* \return int, 0	success,其他值 error，查看错误代码表
			*/
			virtual int RegisterTabPageEvent(ISubWindowEvent *e) = 0;

			//!反注册tab页窗口事件
			/*!
			* \param ISubWindowEvent * e [in] [not NULL]
			* \return int, 0	success,其他值 error，查看错误代码表
			*/
			virtual int UnRegisterTabPageEvent(ISubWindowEvent *e) = 0;

			//!显示tabpage窗口
			/*!
			* \return int, 0 success,其他值 error，查看错误代码表
			*/
			virtual int SelectTabPage() = 0;
		};

		//![interface]临时语音聊天接口
		//!提供创建房间、加入房间、离开房间以及在房间语音聊天的功能
		//!注意：同一时刻只能在一个房间进行语音聊天
		struct ITempAudioSession
		{
			//!创建一个房间
			/*!
			* \param unsigned int &rid [inout], 房间ID
			* \return int, 0	success,其他值 error，查看错误代码表
			*/
			virtual int CreateRoom(unsigned int &rid) = 0;

			//!加入一个房间
			/*!
			* \param unsigned int rid [in], 房间ID
			* \return int, 0	success,其他值 error，查看错误代码表
			*/
			virtual int EnterRoom(unsigned int rid) = 0;

			//!离开一个房间
			/*!
			* \return int, 0	success,其他值 error，查看错误代码表
			*/
			virtual int LeaveRoom() = 0;

			//!开始语音聊天
			/*!
			* \return int, 0	success,其他值 error，查看错误代码表
			*/
			virtual int StartSpeak() = 0;

			//!结束语音聊天
			/*!
			* \return int, 0	success,其他值 error，查看错误代码表
			*/
			virtual int StopSpeak() = 0;

			//!注册临时语音事件
			/*!
			* \param ITempAudioSessionEvent * e [in] [not NULL]
			* \return int, 0	success,其他值 error，查看错误代码表
			*/		
			virtual int RegisterTempAudioSessionEvent(ITempAudioSessionEvent *e) = 0;

			//!反注册临时语音事件
			/*!
			* \param ITempAudioSessionEvent * e [in] [not NULL]
			* \return int, 0	success,其他值 error，查看错误代码表
			*/			
			virtual int UnRegisterTempAudioSessionEvent(ITempAudioSessionEvent *e) = 0;

			//!获取音量
			/*!
			* \param unsigned int& vol [out]，音量值：[0, 100]
			* \return int, 0	success,其他值 error，查看错误代码表
			*/			
			virtual int GetVolume(unsigned int& vol) = 0;

			//!设置音量
			/*!
			* \param unsigned int vol [in]，音量值：[0, 100]
			* \return int, 0	success,其他值 error，查看错误代码表
			*/			
			virtual int SetVolume(unsigned int vol) = 0;

			//!获取当前使用临时语音的APPID
			/*!
			* \param unsigned int &owner [out]，返回值等于0表示当前没有APP使用临时语音
			* \return int, 0	success,其他值 error，查看错误代码表
			*/			
			virtual int GetOwner(unsigned int& owner) = 0;
		};

		//![interface]应用互动接口
		//!提供邀请者的信息
		struct IInteraction
		{
			enum InviteType
			{
				EINVITE_NONE = 0,
				EINVITE_SUB_CHANNEL_ALL,    //邀请子频道的所有人
				EINVITE_SUB_CHANNEL_USER    //邀请制定用户(将会打开窗口来选择用户列表)
			};
			//!获取邀请者uid,只有在被邀请启动应用才能获取成功
			/*!
			* \param unsigned int &uid [inout], 邀请者 uid
			* \return int, 0	success,其他值 error，查看错误代码表
			*/
			virtual int GetInviter(unsigned int &uid) = 0;

			//!发起一次邀请
			/*!
			* \param InviteType [in] 邀请类型,
			* \return int, 0	success,其他值 error，查看错误代码表
			*/
			virtual int Invite(InviteType type) = 0;

			//!应用交互启动的参数
			/*!
			* \param char *params [inout],交互方式启动的运行参数字符串地址,编码：UTF-8
			* \param int size [inout], 字符串大小  size <= 512
			* \return int, 0 success,其他值 error，查看错误代码表
			*/
			virtual int GetRunParams(char *params, unsigned int& size) = 0;

			//!关注
			/*!
			* \param unsigned int uid， 关注某人的uid
			* \return int, 0	success,其他值 error，查看错误代码表
			*/
			//virtual int Follow(unsigned int uid) = 0;

			//!取消关注
			/*!
			* \param unsigned int uid， 关注某人的uid
			* \return int, 0	success,其他值 error，查看错误代码表
			*/
			//virtual int UnFollow(unsigned int uid) = 0;


			//!获取粉丝人数
			/*!
			* \param unsigned int count， 粉丝人数
			* \return int, 0	success,其他值 error，查看错误代码表
			*/			
			//virtual int GetFansCount(unsigned int &count) = 0;

			//!我是否是某人粉丝
			/*!
			* \param unsigned int uid， 询问对象的uid
			* \param short & isfans, 等于0 不是粉丝， 大于 0 是粉丝
			* \return int, 0	success,其他值 error，查看错误代码表
			*/
			//virtual int IsFans(unsigned int uid, short &isfans) = 0;

		};

		//![interface]模版接口
		//!提供创建模版，关闭模版等接口
		struct IChannelTemplate
		{
			/*!创建模板
			/*!
			* \param unsigned long &h [inout], Window HANDLE 引用
			* \param unsigned int &x [inout], 子窗口 x 位置
			* \param unsigned int &y [inout], 子窗口 y 位置
			* \param unsigned int &width [inout], 子窗口 宽
			* \param unsigned int &height [inout], 子窗口  高
			* \param bool &visible [inout], 是否显示
			* \return int, 0	success,其他值 error，查看错误代码表
			*/	
			virtual int CreateTemplate(unsigned long &h, 
				unsigned int &x, 
				unsigned int &y, 
				unsigned int &width, 
				unsigned int &height,
				bool &visible) = 0;

			/*!关闭模板
			* \return int, 0 success,其他值 error，查看错误代码表
			*/
			virtual int CloseTemplate() = 0;

			//!注册模板事件
			/*!
			* \param ISubWindowEvent * e [in] [not NULL]
			* \return int, 0	success,其他值 error，查看错误代码表
			*/	
			virtual int RegisterTemplateEvent(ISubWindowEvent *e) = 0;

			//!反注册模板事件
			/*!
			* \param ISubWindowEvent * e [in] [not NULL]
			* \return int, 0	success,其他值 error，查看错误代码表
			*/	
			virtual int UnRegisterTemplateEvent(ISubWindowEvent *e) = 0;

			enum ETemplateType
			{
				FULL_TEMPLATE = 0,//全频道模板,默认的
				EMBED_TEMPLATE    //嵌入式模板
			};
			/*!创建模板  全频道模式
			* \param ETemplateType type,模板窗口类型
			* \param unsigned long &h [inout], Window HANDLE 引用
			* \param unsigned int &x [inout], 子窗口 x 位置
			* \param unsigned int &y [inout], 子窗口 y 位置
			* \param unsigned int &width [inout], 子窗口 宽
			* \param unsigned int &height [inout], 子窗口  高
			* \param bool &visible [inout], 是否显示
			* \return int, 0	success,其他值 error，查看错误代码表
			*/
			virtual int CreateTemplateEx(ETemplateType type,unsigned long &h, 
				unsigned int &x, 
				unsigned int &y, 
				unsigned int &width, 
				unsigned int &height,
				bool &visible) = 0;
		};

		//![interface]财务接口
		//!财务相关接口
		struct IFinance
		{
			/*!付费请求
			/*!
			* \param unsigned int64 sn [in],请求流水号
			* \param unsigned int srvid [in],应用服务器id
			* \param unsigned int money [in],扣费数目，单位为 0.01Y币，也就是1分钱
			* \param unsigned char moneytype [in],10为Y币 未来支持其余种类货币
			* \param const char* token [in],该次消费请求的token
			* \param EFinanceMode mode,模式，参考EFinanceMode定义
			* \return int, 0	success,仅仅表示发送请求成功，不表示扣费成功，是否扣费成功请参考IFinanceEvent事件
			*/	
			virtual int Buy( unsigned long long sn, 
				unsigned int srvid, 
				unsigned int money, 
				unsigned char moneytype, 
				const char* token,
				YYEnums::EFinanceMode mode) = 0;

			/*!付费请求
			/*!
			* \param unsigned int toid [in],接受者的uid，如果自己购买则填0
			* \param unsigned int64 sn [in],请求流水号
			* \param unsigned int srvid [in],应用服务器id
			* \param unsigned int giftid [in],礼物id
			* \param unsigned int giftcount [in],礼物数量
			* \param unsigned char moneytype [in],10为Y币 未来支持其余种类货币
			* \param const char* token [in],该次消费请求的token
			* \param EFinanceMode mode,模式，参考EFinanceMode定义
			* \return int, 0	success,仅仅表示发送请求成功，不表示扣费成功，是否扣费成功请参考IFinanceEvent事件
			*/
			virtual int BuyGifts(unsigned int toid, 
				unsigned long long sn, 
				unsigned int srvid, 
				unsigned int giftid, 
				unsigned int giftcount, 
				unsigned char moneytype, 
				const char* token,
				YYEnums::EFinanceMode mode) = 0;

			//!注册事件
			/*!
			* \return int, 0	success,其他值 error，查看错误代码表
			*/	
			virtual int RegisterFinanceEvent(IFinanceEvent *e) = 0;

			//!反注册模板事件
			/*!
			* \return int, 0	success,其他值 error，查看错误代码表
			*/	
			virtual int UnRegisterFinanceEvent(IFinanceEvent *e) = 0;

			//!打开充值页面
			/*!
			* \return int, 0	success,其他值 error，查看错误代码表
			*/	
			virtual int Recharge() = 0;

			/*!付费请求
			/*!
			* \param unsigned int64 sn [in],请求流水号
			* \param unsigned int srvid [in],应用服务器id
			* \param double money [in],扣费数目，单位是元，保留两位小数点，否则返回错误
			* \param unsigned char moneytype [in],10为Y币 未来支持其余种类货币
			* \param const char* token [in],该次消费请求的token
			* \param EFinanceMode mode,模式，参考EFinanceMode定义
			* \return int, 0	success,仅仅表示发送请求成功，不表示扣费成功，是否扣费成功请参考IFinanceEvent事件
			*/	
			virtual int BuyByYuan( unsigned long long sn, 
				unsigned int srvid, 
				double money,
				unsigned char moneytype, 
				const char* token,
				YYEnums::EFinanceMode mode) = 0;
		};

        /*![interface] IChannelChat接口
		*/
		struct IChannelChat
		{
			//!注册聊天事件
			/*!
			\param IChannelChatInternalEvent * event 聊天事件
			/* \return 0 sucess, failed 查看错误代码
			*/
			virtual int RegisterChatEvent(IChannelChatEvent * event) = 0;
			
			//!注销聊天事件
			/*!
			\param IChannelChatInternalEvent * event 聊天事件
			/* \return 0 sucess, failed 查看错误代码
			*/
			virtual int UnRegisterChatEvent(IChannelChatEvent * event) = 0;
		};

		/*![interface] IVideo接口
		*/
		struct IVideo
		{
			//!获取视频直播控制权
			/*!
			/* \return 0 sucess, failed 查看错误代码
			*/
			virtual int Lock() = 0;

			//!释放视频直播控制权
			/*!
			/* \return 0 sucess, failed 查看错误代码
			*/
			virtual int UnLock() = 0;

			//!获取摄像头列表
			/*!
			\param const char *** cameras 摄像头名称数组，内存由系统分配和释放
			\param int * count 摄像头名称数组大小
			/* \return 0 sucess, failed 查看错误代码
			*/
			virtual int GetCameraList(const char *** cameras, int * count) = 0;

			//!开始直播
			/*!
			\param unsigned int hwnd 视频窗口句柄
			\param unsigned int camera 使用的摄像头索引号
			/* \return 0 sucess, failed 查看错误代码
			*/
			virtual int StartPublish(unsigned int hwnd, unsigned int camera) = 0;

			//!停止直播
			/*!
			/* \return 0 sucess, failed 查看错误代码
			*/
			virtual int StopPublish() = 0;

			//!注册视频直播事件
			/*!
			\param IVideoEvent * event 视频直播事件
			/* \return 0 sucess, failed 查看错误代码
			*/
			virtual int RegisterVideoEvent(IVideoEvent * event) = 0;

			//!注销视频直播事件
			/*!
			\param IVideoEvent * event 视频直播事件
			/* \return 0 sucess, failed 查看错误代码
			*/
			virtual int UnRegisterVideoEvent(IVideoEvent * event) = 0;
		};
	}
}

#endif //!_DW_OPENPLATFORM_CLIENT_API_H_

//!! IYY SDK event  定义
/*! 接口说明 platform api 事件 接口, 使用者继承对应的Event接口实现之，注册到指定的接口
	注意多线环境，event 只会在APi初始化线程返回 ，建议在UI线程初始化API
 * @file dw-api-events.h
 * @create date 2011-12-01
 * @modify data 2011-12-01
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

#ifndef _DW_PLATFORM_API_EVENTS_H_
#define _DW_PLATFORM_API_EVENTS_H_

#include "yy-api-types.h"

namespace yy
{

extern "C"
{
	//![event] YY接口事件
	struct IYYEvent
	{
		//!YY Api 初始化事件
		/*!
		 * \param result [in] ,0 sucess, 其他值，请查看错误代码
		*/		
		virtual void OnInitialize(int result) = 0;

		/*! 当被应用盒子或者其他应用入口点击时，产生的事件
		 * \param active_code [in], active_code = 0 ,在应用被点击,其他值暂时未开放
		*/		
		virtual void OnActive(int active_code) = 0;

		//!YY APi YY应用退出
		/*!
		 * \param exit_code [in] ,0 sucess, 其他值，请查看错误代码
		*/	
		virtual void OnExit(int exit_code) = 0;
	};

	//![event] 用户事件
	struct IUserEvent  
	{
		//!用户信息改变
		/*!
		 * \param info [in] ,改变后的用户信息
		*/
		virtual void OnUserInfoChanged(IUserInfo *info) = 0;
	};
	
	//![event] 频道信息获取事件
	struct IChannelEvent
	{
		//!子频道跳转事件
		/*!
		 * \param unsigned int departed_id [in] ,原来子频道id
		 * \param unsigned int now_id	[in]	,现在子频道id
		*/
		virtual void OnFocusChannelChannged(unsigned int departed_id, unsigned int now_id) = 0;

		//!频道信息改变事件
		/*! 
		* \param IChannelInfo *info[in] ,改变后的频道信息
		*/
		virtual void OnChannelInfoChannged(IChannelInfo *info) = 0;

		//!删除子频道事件
		/*!
		* \param unsigned int cid [in], 子频道id
		*/
		virtual void OnSubChannelDel(unsigned int cid) = 0;

		//!添加子频道事件
		/*!
		 * \param unsigned int cid [in], 新增的子频道id，
		 * \param unsigned int pcid [in], 父频道id
		*/
		virtual void OnSubChannelAdd(unsigned int cid, unsigned int pcid) = 0;

		//!用户进入频道事件
		/*!
		 * \param unsigned int uid [in], 进频道 用户id，
		 * \param unsigned int cid [in], 进入频道id
		*/
		virtual void OnUserEnterChannel(unsigned int uid, unsigned int cid) = 0;

		//!用户退出频道事件
		/*!
		* \param unsigned int uid [in], 进频道 用户id，
		* \param unsigned int cid [in], 进入频道id
		*/
		virtual void OnUserLeaveChannel(unsigned int uid, unsigned int cid) = 0;
	};
	
	//![event] 语音事件
	struct IAudioEvent
	{
		//!录音错误事件
		/*!
		* \param int err_code [in], 录音错误代码，参考错误代码表
		*/
		virtual void OnRecordErr(int err_code) = 0;
	    
		//!录音完成事件
		/*!
		* \param int result [in], 录音结果
		* \param const char *file_name[in],录音文件名
		*/
		virtual void OnRecordFinished(int result, const char *file_name) = 0;
	};

	//![event] 频道用户列表右键菜单事件
	struct IChannelUserPopMenuEvent
	{
		//!右键用户菜单事件
		/*!
		* \param unsigned int uid [in], 用户id
		* \param unsigned int cid [in], 用户所在频道id
		*/
		virtual void OnClicked(unsigned int uid, unsigned int cid) = 0;
	};

	//![event] 网络事件
	struct INetEvent
	{
		//!连接事件
		/*!
		* \param int result [in],连接结果代码, 0:success ,其他值 error, 参考DWApi错误代码表
		*/
		virtual void OnConnected(int result) = 0;
		
		//!断开事件
		/*!
		* \param int result, 0:主动断开, 其他值error, 误参考 DWApi错误代码表
		*/
		virtual void OnClosed(int result) = 0;

    //!收到广播数据包
		/*!
		* \param const char *data	[in], 收到数据包地址
		* \param unsigned int size	[in], 收到数据包的大小
		*/
		virtual void OnRecv(const char *data, unsigned int size) = 0;
	};

	//![event]应用消息事件
	struct IChannelAppLinkEvent
	{
		//!链接点击事件
		/*!
		* \param unsigned int cookie 	[in],链接消息用户自定义值
		*/
        virtual void OnAppLinkClicked(unsigned int cookie) = 0;

		//!多连接点击事件
		/*!
        * \user_data                    [in],用户定义的链接数据
		* \param unsigned int cookie 	[in],链接消息用户自定义值
		*/
        virtual void OnAppLinkExClicked(unsigned int user_data, unsigned int cookie) = 0;
	};
	
	//![event]麦序相关列表事件
	//!当前所在子频道 用户上下麦序和清空麦序，以及某人移动麦序的事件
	struct IMicListEvent 
	{
		//!有人加入麦序
		/*!
		* \param unsigned int uid [in], 加入麦序用户的id
		*/
	    virtual void OnUserJoin(unsigned int uid) = 0;
		
		//!有人离开麦序
		/*!
		* \param unsigned int uid [in], 下麦用户的id
		*/
		virtual void OnUserLeave(unsigned int uid) = 0;
		
		//!麦序上的人移动
		/*!
		* \param unsigned int uid,麦序移动的用户的id
		* \param unsigned int after_uid,移动到某个用户后面的id
		*/
		virtual void OnUserMove(unsigned int uid, unsigned int after_uid) = 0;
		
		//!清空麦序事件
		/*! 
		*/
		virtual void OnClear() = 0;
	};

	//![event]麦序相关列表事件 扩展1
	//!当前所在子频道 用户上下麦序和清空麦序，以及某人移动麦序的事件
	struct IMicListEvent2
	{
		//!加入连麦事件
		/*!
		/param unsigned int uid, 加入连麦的用户 uid
		*/
		virtual void OnUserMicLinked(unsigned int uid) = 0;

		//!离开连麦事件
		/*!
		/param unsigned int uid, 离开连麦的用户 uid
		*/
		virtual void OnUserMicUnlinked(unsigned int uid) = 0;

		//!频道麦模式改变事件
		/*!
		/param EMicListMode cur_mode, 改变之后的模式
		/param EMicListMode reserve,  保留参数
		*/
		virtual void OnMicListModeChanged(YYEnums::EMicListMode cur_mode, YYEnums::EMicListMode reserve) = 0;
	};

	//![event]麦序管理事件
	//!当前所在子频道 用户上下麦序和清空麦序，以及某人移动麦序的事件
	struct IMicListMgrEvent
	{
		//!控麦事件，控麦状态事件
		/*!
		 /param unsigned int uid, 操作者 uid
		*/
		virtual void OnMicListControlled(unsigned int adminUid) = 0;

		//!放麦事件，控麦状态事件
		/*!
		 /param unsigned int uid, 操作者 uid
		*/
		virtual void OnMicListReleased(unsigned int adminUid) = 0;

		//!开麦事件, 禁止抢麦状态事件
		/*!
		/param unsigned int uid, 操作者 uid
		*/
		virtual void OnEnableJoinMicList(unsigned int adminUid ) = 0;

		//!禁麦事件, 禁止抢麦状态事件
		/*!
		/param unsigned int uid, 操作者 uid
		*/
		virtual void OnDisableJoinMicList(unsigned int adminUid ) = 0;

		//!麦首时间改变事件
		/*!
		/param unsigned int adminUid, 操作者uid
		/param unsigned int uid, 被操作者uid
		/param unsigned int seconds, 新的麦序时间uid
		*/
		virtual void OnMicListTimeChanged(unsigned int adminUid, unsigned int uid, unsigned int seconds) = 0;

		//!用户发言状态改变事件
		/*!
		/param unsigned int uid, 用户uid
		/param bool speaking, 是否在说话
		*/
		virtual void OnUserSpeakingStateChanged( unsigned int uid, bool speaking ) = 0;

		//!麦序提醒事件
		/*!
		/param unsigned int adminUid, 操作者uid
		*/
		virtual void OnMicListNotification( unsigned int adminUid ) = 0;
	};

	//![event] 子窗口事件
	struct ISubWindowEvent
	{
		//!子频道窗口显示事件
		/*!
		 * \param unsigned int x [in], 子频道窗口x坐标
		 * \param unsigned int y [in], 子频道窗口y坐标
		 * \param unsigned int width [in], 子频道窗口宽度
		 * \param unsigned int height [in], 子频道窗口高度
		*/
		virtual void OnShow(unsigned int x, 
			unsigned int y,
			unsigned int width,
			unsigned int height) = 0;
		
		//!子频道窗口隐藏事件
		virtual void OnHide() = 0;
		
		//!子频道窗口大小改变事件
		/*!
		 * \param unsigned int x [in], 子频道窗口x坐标
		 * \param unsigned int y [in], 子频道窗口y坐标
		 * \param unsigned int width [in], 子频道窗口宽度
		 * \param unsigned int height [in], 子频道窗口高度
		*/
		virtual void OnSizeChanged(unsigned int x, 
			unsigned int y,
			unsigned int width,
			unsigned int height) = 0;

		virtual void OnClosed() = 0;
	};

	//![event] 临时语音聊天事件
	struct ITempAudioSessionEvent
	{
		//!用户进入房间事件	
		/*!
		* \param unsigned int rid [in], 房间ID
		* \param unsigned int uid [in], 用户ID
		*/
		virtual void OnUserEnterRoom(unsigned int rid, unsigned int uid) = 0;
		
		//!用户离开房间事件	
		/*!
		* \param unsigned int rid [in], 房间ID
		* \param unsigned int uid [in], 用户ID
		*/
		virtual void OnUserLeaveRoom(unsigned int rid, unsigned int uid) = 0;
	};

	//![event] 财务相关事件
	struct IFinanceEvent
	{
		//!扣费相应	
		/*!
		*/
		virtual void OnBuyResponse(unsigned long long sn, int ret, YYEnums::EFinanceMode mode) = 0;
		
		//!买礼物相应	
		/*!
		*/
		virtual void OnBuyGiftsResponse(unsigned long long sn, int ret, YYEnums::EFinanceMode mode) = 0;
	};

	/*![interface] IM好友事件
	*/
	struct IIMExEvent
	{
		/*!用户信息改变
		*/
		virtual void OnUserInfoChanged(unsigned int uid, unsigned int flags) = 0;

		/*!
		*/
		virtual void OnBuddyAdd(unsigned int uid) = 0;

		/*!
		*/
		virtual void OnBuddyRemove(unsigned int uid) = 0;

		/*!
		*/
		virtual void OnAllBuddiesRemove() = 0;
	};

    /*! [interface] 频道公屏聊天接口
	 * 
	*/
	struct IChannelChatEvent
	{
		//! 公屏消息回调
		/*!
		\param unsigned int uid 消息发送方
		\param const char * msg 消息
		\param unsigned int size 消息体大小 max size 512k
		*/
		virtual void OnChat(unsigned int uid, const char * msg, unsigned int size) = 0;

		//! 么聊消息回调
		/*!
		\param unsigned int uid 消息发送方
		\param const char * msg 消息
		\param unsigned int size 消息体大小 max size 512k
		*/
		virtual void OnChatFrom(unsigned int uid, const char * msg, unsigned int size) = 0;
	};

	/*! [interface] 视频直播事件
	* 
	*/
	struct IVideoEvent
	{
		//! 摄像头状态变化事件
		/*!
		\param unsigned status 0--摄像头就绪，1--摄像头就绪暂不可用（被别的程序占用），2--摄像头无法支持
		*/
		virtual void OnCameraStatus(unsigned status) = 0;

		//! 么聊消息回调
		/*!
		\param unsigned status 0--成功上传视频，1--上传视频失败，2--上传视频成功后，发送视频数据过程中，出现错误
		*/
		virtual void OnPublishStatus(unsigned status) = 0;
	};
}

}
#endif //!_DW_PLATFORM_API_EVENTS_H_
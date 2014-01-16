//! IYY SDK typde  定义
/*! 接口说明
 * @file dwop-api-com.h
 * @create date 2011-11-30
 * @modify data 2012-10-25
 * @author simon.lv
 * @version 1.8
 * @modify list
 * ............................................................................
 * ............................................................................
 * yy open platform client api, 基于异步的 yyplatform 网络通信协议
 * 广州华多网络科技有限公司 版权所有 (c) 2005-2011 DuoWan.com [多玩游戏]

 ******************************************************************************
 * 详细接口文档参考open.yy.com
 *******************************************************************************/

#ifndef _DW_OPENPLATFORM_API_TYPES_H_
#define _DW_OPENPLATFORM_API_TYPES_H_

//!版本信息
//!sdk version 1.15
#define DW_API_VERSION (1 << 16 | 15)

#define YYAPI_INVALID_UID ((unsigned int)0)
#define YYAPI_INVALID_KEY YYAPI_INVALID_UID

namespace yy
{

extern "C"
{
	//![interface]YY COM接口，类似COM机制，引用指针时调用AddRef(),释放指针 Release()
	//!不支持QueryInterface 和 IDisPathcer等其他COM特性
	struct IYYUnk
	{
		//!添加引用
		/*!
		*/
		virtual void AddRef() = 0;
		
		//!释放引用
		/*!
		*/
		virtual void Release() = 0;
	};

	//![interface] YY用户信息
	struct IUserInfo : public IYYUnk
	{
		enum EUserRole
		{
			//!无效角色
			NUlL_ROLE			= 0,
			//!访问者 VS 灰马
			VISITOR				= 20,	
			//!普通成员 N 白马
			NORMAL				= 25,	
			//!临时嘉宾	TMVIP 
			TMPVIP				= 66,
			//!嘉宾	VIP 绿马	
			VIP					= 88,
			//!会员	R 蓝马
			MEMBER				= 100,
			//!二级子频道管理员 CA 红马
			CMANAGER			= 150,
			//!一级子频道管理员 
			PMANAGER			= 175,
			//!管理员			 MA 黄马
			MANANGER			= 200,
			//!副会长			
			VICE_OWNER			= 230,
			//!频道所有者   OW    紫马
			OWNER				= 255,
			//!客服
			CUSTOMER_SERVICE	= 300,
			//!DW官方人员		黑马  
			OFFICIAL			= 1000
		};
				
		//!获取uid
		/*!
		* /return unsigned int, 用户id
		*/
		virtual unsigned int GetUid() const = 0;

		/*!获取用户im id
		* \return IM号码
		*/
		virtual unsigned int GetIMId() const = 0;

		/*!	获取用户性别
		* \return unsigned int 性别(0:男  1:女)
		*/
		virtual unsigned int GetSex() const = 0;	

		/*!获取用户角色
		* \return unsigned int 在当前子频道中的角色
		*/
		virtual unsigned int GetRole() const = 0;

		/*!获取用户积分
		* \return unsigned int, 个人积分
		*/
		virtual unsigned int GetPoints() const = 0;

		/*!	获取用户等级
		* \return unsigned int, 个人等级
		*/
		virtual unsigned int GetLevel() const = 0;		
		
		//!获取用户昵称 
		/*!
		* \param name [inout] 保存用户名 字符数组地址,编码 CP_UTF8
		* \param size[in]   数组大小 max size = 40 * 2,
		* \return int, 0 success, 其他值error
		*/
		virtual int	GetName(char *name, int size) const = 0;

		//!用户个性签名
		/*!
		* \param sign [inout] 保存公告信息 字符串地址,编码 CP_UTF8
		* \param size [in] 数组大小 <= 100 * 2
		* \return int, 0 success, 其他值error, 参考错误代码
		*/
		virtual int	GetSign(char *sign, int size) const = 0;	//! 用户个性签名

		//!是否为付费会员
		/*!
		* \return bool, true 付费会员, false 非付费会员
		*/
		virtual bool IsVip() const = 0;

		//!会员等级
		/*!
		* \return int, 会员等级，非vip 等级为 0
		*/
		virtual int GetVipLevel() const = 0;

		//!获取公会贡献值
		/*!
		* \return int, 会员等级，非vip 等级为 0
		*/
		virtual int GetContribution() const = 0;
	};

	//![interface] 频道信息 
	struct IChannelInfo : public IYYUnk
	{
		enum EChannelType
		{
			GAME = 0,//游戏
			ENTERTAINMENT,//娱乐
            OTHER, // 其他
			EDUCATION,//教育
		};
		//! 获取频道长位id
		/*!
		* \return int,
		*/
		virtual unsigned int GetLongId() const = 0;

		//! 获取频道短位id
		/*!
		* \return int,频道短位id
		*/
		virtual unsigned int GetShortId() const = 0;
		
		//!频道昵称
		/*!
		* \param name [out] 保存频道名 字符数组地址 ,编码 CP_UTF8
		* \param size[in]   数组大小 , 值范围 [0,40]
		* \return int, 0 success, 其他值error, 参考错误代码
		*/
		virtual int	GetName(char *name, int size) const = 0;

		//!当前子频道用户数
		/*!
		* \return unsigned int, 返当前子频道用户数
		*/
		virtual unsigned int GetUserCount() const = 0;

		//!频道全量用户数
		/*!
		* \return unsigned int,返回频道全部用户总数
		*/
		virtual unsigned int GetTotalUserCount() const = 0;

		//!获取当前频道类型
		/*!
		* \return EChannelType,频道类型
		*/
		virtual EChannelType GetChannelType() const = 0;

		//!获取频道积分
		/*!
		* \return unsigned __int64,频道积分
		*/
		virtual unsigned __int64 GetChannelPoints() const = 0;
	};

	//![interface] 序用户信息
	struct IMicListItem
	{
		//!获取当前节点用户Id
		/*!
		* \return int,用户id
		*/		
		virtual unsigned int GetUserId() const = 0;

		//!下一个节点
		/*!
		* \return IMicListItem *,下一个麦序用户的节点
		*/	
		virtual const IMicListItem * GetNext() const = 0;
	};

	//![interface] 频道树节点 
	struct IChannelNode
	{
		//!获取频道树 root 节点
		/*!
		* \return IChannelNode *,频道树 root 节点
		*/			
		virtual const IChannelNode * GetRoot() const = 0;
	  
	  //!获取频道树父节点
	  /*!
		* \return IChannelNode *,频道节点 成功 有效指针，NULL error
	  */
		virtual const IChannelNode * GetParent() const = 0;
	  
	  //!获取频子节点
	  /*!
	   * \param unsigned int index,子节点 索引值, 从零开始，值必须小于 子节点数量,从0开始索引
		* \return IChannelNode *,频道节点 成功 有效指针，NULL error
	  */	  
		virtual const IChannelNode * GetChildAt(unsigned int index) const = 0;

		//!通过获取子节点，子节点的子节点也可以获取
		/*!
	    * \param unsigned int cid, 过 频道ID获取节点
		* \return IChannelNode *,频道节点 成功 有效指针，NULL error			
		*/
		virtual const IChannelNode * GetChild(unsigned int cid) const = 0;
	  
	  //!获取频子节点数量
	  /*!
		* \return int, 值范围 [0,4000)
	  */	  
		virtual const unsigned int GetCount() const = 0;
	  
	  //!获取当前节点频道id
	  /*!
		* \return int, 当前节点频道id
	  */		  
		virtual const unsigned int GetCid() const = 0;
	};

	//![interface] 云存储数据结构体
	struct ICloudData : public IYYUnk
	{
		//!获取当前数据 唯一id
	  /*!
		* \return unsigned __int64, 唯一id
	  */		
		virtual unsigned __int64 GetUniqueKey() const = 0;

		//!获取数据创建者的Uid
	  /*
		* \return unsigned int, 唯一id
	  */
		virtual unsigned int GetCreatorUid() const = 0;

		//!获取当前数据 最后一次更新时间
	  /*!
		* \return unsigned __int64 更新时间
	  */
		virtual unsigned __int64 GetUpdateTime() const = 0;

		//!获取当前数据 创建时间
	  /*!
		* \return unsigned __int64 创建时间
	  */
		virtual unsigned __int64 GetCreateTime() const = 0;
		
		//!获取当前数据 第一个int值
	  /*!
		* \return unsigned int, 第一个int值
	  */				
		virtual unsigned int GetIntValue1() const = 0;
		
		//!获取当前数据 第二个int值
	  /*!
		* \return unsigned int, 第二个int值
	  */			
		virtual unsigned int GetIntValue2() const = 0;
		
	  //!获取字符串值
	  /*!
		* \param char *data, 字符串
		* \return int, 0 success, 其他值error, 参考错误代码
	  */			
		virtual const char * GetStringValue() const = 0;
		
	  //!获取2进制流首地址
	  /*!
		* \return char *, 2进制数据首地址
	  */			
		virtual const char * GetBinaryValue() const = 0;
		
	  //!获取2进制流 大小
	  /*!
		* \return char *, 2进制数据大小， 值范围 [0,4096]
	  */	
		virtual int GetBinraySize() const = 0;
		
	  //!设置第一个int值
	  /*!
	  * \param unsigned int value, int 值
		* \return int, 0 success, 其他值error, 参考错误代码
	  */	
		virtual int SetIntValue1(unsigned int value) = 0;
		
		//!设置第二个int值
		/*!
		* \param unsigned int value int的值
		* \return int, 0 success, 其他值error, 参考错误代码
		*/
		virtual int SetIntValue2(unsigned int value) = 0;
		
		//!设置字符串值
		/*!
		* \param const char *data, 字符串值
		* \return int, 0 success, 其他值error, 参考错误代码
		*/
		virtual int SetStringValue(const char *data) = 0;
		
		//!设置2进制值
		/*!
		* \param const char *data, 字符串值
		* \param const unsigned int size, 二进制数据大小 ,值范围 (0,4096]
		*/		
		virtual int SetBinrayValue(const char *data, unsigned int size) = 0;		
	};

	//![interface] 云存储数据 列表
	//!用于查询数据 返回结果，ICloudData 的列表
	struct ICloudDataList
	{
		//!使用索引值获取云存储数据
		/*!
		* \param unsigned int index, 索引值,从0开始索引
		* \return ICloudData *, 有效指针 success, 其他值 error
		*/
		virtual const ICloudData * GetDataAt(unsigned int index) = 0;
		
		//!获取云存储list数量
		/*!
		* \return unsigned int, 列表数量
		*/
		virtual unsigned int GetCount() = 0;
	};

	//![interface] 云存储过滤器
	//!提供对云存储ICloudData 第一个Int和第二个Int 和 创建时间和修改 的查询 操作
	struct ICloudFilter : public IYYUnk
	{
		enum EField
		{
			//!无效字段
			FIELD_NONE = 0,
			//!key 唯一键 字段
			UNIQUE_KEY_FIELD = 1,	
			//!uid 字段
			USER_ID_FIELD = 2,
			//!扩展int1 字段
			External_VALUE1 = 3,
			//!扩展int2 字段
			External_VALUE2 = 4,
			//!创建时间
			FIELD_CREATE_TIME = 5,
			//!更新时间
			FILED_UPDATE_TIME = 6
		};

		enum EFilterOperator
		{
			//! 无效操作
			FILTER_OP_NONE = 0,
			//! = 等于
			FILTER_OP_EQ = 1,
			//! >= 大于等于
			FILTER_OP_GE = 2,
			//! <= 小于等于	
			FILTER_OP_LE = 3,
			//! = 等于
			FILTER_OP_GREATER = 4,
			//! < 小于
			FILTER_OP_LESS = 5
		};

		enum EFilterCondition
		{
			//!无效条件
			FILTER_CD_NONE = 0,
			//! 条件 与 and 
			FILTER_CD_AND = 1,
			//! 条件 或 or
			FILTER_CD_OR
		};

		//!增加 过滤条件
		/*!
		* \param const EField field,  滤条件字段
		* \param const EFilterOperator op, 过滤条件字段 操作
		* \param const unsigned int filter_value, 过滤条件操作 值
		* \param const EFilterCondition condition,条件 操作
		* \return int ,0 success, 其他值 error
		*/
		virtual int AddFiter(const EField field,
			const EFilterOperator op,
			const unsigned __int64 filter_value,
			const EFilterCondition condition) = 0;
	};

	//![interface] 新应用消息
	struct IAppMsg : public IYYUnk
	{
		enum EMsgItemType
		{
			EMSG_NONE = 0,
			EMSG_TEXT,
			EMSG_LINK, 
		};

		virtual int AddMsgItem(const char *msg_item, 
			unsigned int size,
			unsigned int user_data,
			EMsgItemType type) = 0;

		virtual int SetCookie(unsigned int cookie) = 0;

        virtual int SetKey( const char *key, unsigned int size ) = 0;
	};

	//!api各种枚举类型
	struct YYEnums
	{
		//!麦序模式
		enum EMicListMode
		{
			FreeMode = 0,
			ManagerMode,
			QueueMode,
		};

		//!财务模式
		enum EFinanceMode
		{
			//!正常模式
			NormalMode = 0,
			//!测试模式
			TestMode
		};
	};
}

}
#endif //!_DW_OPENPLATFORM_API_TYPES_H_
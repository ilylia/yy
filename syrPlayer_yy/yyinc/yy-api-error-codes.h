//! IYY SDK 错误代码表
/*! 接口说明
* @file dw-api-error-codes.h
* @create date 2011-11-30
* @modify data 2011-11-30
* @author simon.lv
* @version 1.0
* @modify list
* ............................................................................
* ............................................................................
* yy open platform client api, 基于异步的yyplaitform 网络通信协议
* 广州华多网络科技有限公司 版权所有 (c) 2005-2011 DuoWan.com [多玩游戏]

******************************************************************************
* 详细接口文档参考open.yy.com
*******************************************************************************/

#ifndef _DW_PLATFORM_API_ERROR_CODES_H_
#define _DW_PLATFORM_API_ERROR_CODES_H_

typedef int YYRESULT;

const int yy_success	= 0x00000000;                       // 成功(无错)
const int yy_e_unknown	= 0x00000001;                       // 未知错误

const int yy_e_buffer_length_is_out_of_limit	= 0x0002;   // 数据超长
const int yy_e_user_not_found					= 0x0003;   // 用户没找到
const int yy_e_channel_not_found				= 0x0004;   // 频道没找到

const int yy_e_cloud_add_data_failed			= 0x0005;   // 简单存储添加数据失败
const int yy_e_cloud_quary_data_failed			= 0x0006;   // 简单存储查询失败
const int yy_e_cloud_wrong_format				= 0x0007;   // 简单存储参数格式不对

const int yy_e_empty_msg						= 0x0008;   // 消息为空
const int yy_e_empty_user_list					= 0x0009;   // 用户列表为空
const int yy_e_user_count_out_of_limit			= 0x000a;   // 用户列表超长

const int yy_e_no_permission					= 0x000b;   // 没有操作权限
const int yy_e_value_out_of_range				= 0x000c;   // 参数超出预定义边界
const int yy_e_not_found						= 0x000d;   // 未找到目标

const int yy_e_string_is_too_long				= 0x000e;   // 字符串超长

const int yy_e_parameter_value_not_defined      = 0x000f;   // 参数值未定义

const int yy_e_can_not_open_ui                  = 0x0010;   // 不能打开对应的UI界面

const int yy_e_improper_channel_mic_mode        = 0x0011;    // 当前频道麦模式无法进行此操作

// 临时语音错误代码
const int yy_e_temp_audio_session_inner_exception = 0x9000;
const int yy_e_temp_audio_session_illegal_param = 0x9001;
const int yy_e_temp_audio_session_not_in_room = 0x9002;
const int yy_e_temp_audio_session_failed_to_leave_pre_room = 0x9003;
const int yy_e_temp_audio_session_failed_to_login = 0x9004;
const int yy_e_temp_audio_session_failed_to_create_room = 0x9005;
const int yy_e_temp_audio_session_failed_to_enter_room = 0x9006;
const int yy_e_temp_audio_session_failed_to_leave_room = 0x9007;
const int yy_e_temp_audio_session_failed_to_start_speak = 0x9008;
const int yy_e_temp_audio_session_failed_to_stop_speak = 0x9009;
const int yy_e_temp_audio_session_occupied_by_other_app = 0x900a;

//内部网络连接接口错误代码
const int yy_e_connect_id_is_not_exist   = 0xa000;
const int yy_e_failed_to_send_conn_data	 = 0xa001;

// 视频直播错误代码
const int yy_e_video_session_inner_exception = 0xb000;
const int yy_e_video_session_illegal_param = 0xb001;
const int yy_e_video_session_not_in_video_session = 0xb002;
const int yy_e_video_session_occupied_by_other_app = 0xb003;
const int yy_e_video_session_failed_to_get_camera_list = 0xb004;
const int yy_e_video_session_failed_to_set_camera = 0xb005;
const int yy_e_video_session_failed_to_start_video = 0xb006;
const int yy_e_video_session_failed_to_stop_video = 0xb007;
const int yy_e_video_session_no_video_window = 0xb008;
const int yy_e_video_session_bad_camera_index = 0xb009;
const int yy_e_video_session_is_publishing = 0xb00a;

const int yy_api_result_success = 0;
const int yy_api_register_api_failed_already_register = 0x000f0002;
const int yy_api_register_api_failed_time_out = 0x000f0003;
const int yy_api_register_api_failed_platform_not_exist = 0x000f0004;
const int yy_api_register_api_failed_platform_max_connect = 0x000f0005;
const int yy_api_register_api_failed_api_can_not_run = 0x000f0006;
const int yy_api_register_api_failed_api_sys_err	 = 0x000f0007;
const int yy_api_register_api_failed_platform_can_not_connected = 0x000f0008;

const int yy_api_release_failed_platform_exit = 0x000f0009;
const int yy_api_release_failed_yy_exit = 0x000f0010;

const int yy_api_transfer_data_failed_api_not_register_successed = 0x000f0011;
const int yy_api_transfer_data_failed_no_sender_state_error =  0x000f0012;
const int yy_api_transfer_ui_data_failed_api_not_register_successed = 0x000f0013;
const int yy_api_transfer_ui_data_failed_sender_state_error = 0x000f0014;
const int yy_api_get_channel_info_failed_api_not_register_successed = 0x000f0015;
const int yy_api_get_channel_info_failed_sender_state_error = 0x000f0016;
const int yy_api_get_user_info_failed_api_not_register_successed = 0x000f0017;
const int yy_api_get_user_info_failed_sender_state_error = 0x000f0018;

const int yy_api_pre_close_app = 0x000f0018;

const int yy_api_get_user_name_failed_copy_str_err	= 0x000f0110;
const int yy_api_get_user_sign_failed_copy_str_err	= 0x000f0111;

const int yy_api_callback_aready_exist	= 0x000f0600;
const int yy_api_callback_not_exist		= 0x000f0601;
const int yy_api_route_sender_error		= 0x000f0602;
const int yy_api_callback_is_null		= 0x000f0603;

const int yy_api_block_waite_time_out   = 0x000f0700;
const int yy_api_block_watie_error      = 0x000f0701;
const int yy_api_terminal_is_null       = 0x000f0702;
const int yy_api_asyn_call_error        = 0x000f0703;
const int yy_api_disconnect_error        = 0x000f0704;

const int yy_api_not_connect_addon_server    = 0x000f0801;
const int yy_api_string_is_null              = 0x000f0802;
const int yy_api_buffer_is_too_small         = 0x000f0803;
const int yy_api_point_is_null               = 0x000f0804;
const int yy_api_contains_illegal_characters = 0x000f0805;
const int yy_api_call_interface_too_frequently = 0x000f0900;
const int yy_wstr_to_utf8_error    =0x000f0a00;
const int yy_api_block_interface_is_calling = 0x000f0b00;

const int yy_api_file_name_is_illegal = 0x000f0c00; //文件名非法

const int yy_e_api_call_error = 0x0000F230;//十进制值62000  Api调用错误，错误的函数名称，错调用格式会返回此错误。
const int yy_e_api_param_error = 0x0000F231; //十进制值62001 Api调用参数错误，错误的参数个数和类型，会触发此错误。
const int yy_e_api_return_format_error = 0x0000F232; //十进制值62002 Api调用返回值格式错误。

//麦序
const int yy_e_api_in_mike_list = 0x0000F240; //已经在麦序上
const int yy_e_api_out_mike_list = 0x0000F241; //不在麦序上

//
const int yy_e_api_sub_channel_is_not_exist = 0x0000F250; //子频道不存在
const int yy_e_api_already_reception_channel = 0x0000F260; //已经是接待频道了
const int yy_e_api_too_often = 0x0000F271; //接待频道不能反复设置，现在显示5秒调用一个

const int yy_e_api_already_clear_reception_channel = 0x0000F261; //已经移除接待频道了
const int yy_e_api_already_is_this_role = 0x0000F270; //已经是这个马甲
const int yy_e_api_already_is_this_name = 0x0000F271; //已经是这个昵称

//////////////////////////////////////////////////////////////////////////
// async版本错误代码区间 : (0x00010000, 0x0001ffff)
const int yy_e_async_success		= yy_success;
const int yy_e_async_lowbound		= 0x00010000;
const int yy_e_async_internal		= yy_e_async_lowbound + 1;	//0x00010001
const int yy_e_async_failed			= yy_e_async_lowbound + 2;	//0x00010002
const int yy_e_async_notimpl		= yy_e_async_lowbound + 3;	//0x00010003
const int yy_e_async_invalidarg		= yy_e_async_lowbound + 4;	//0x00010004
const int yy_e_async_reducecall		= yy_e_async_lowbound + 5;	//0x00010005
const int yy_e_async_unexpected		= yy_e_async_lowbound + 6;	//0x00010006
const int yy_e_async_outofmem		= yy_e_async_lowbound + 7;	//0x00010007
const int yy_e_async_pending		= yy_e_async_lowbound + 8;	//0x00010008
const int yy_e_async_timeout		= yy_e_async_lowbound + 9;	//0x00010009
const int yy_e_async_sendfailed		= yy_e_async_lowbound + 10;	//0x0001000a
const int yy_e_async_notregisterd	= yy_e_async_lowbound + 11;	//0x0001000b
const int yy_e_async_upbound		= 0x0001ffff;

namespace yy
{
	namespace error
	{
		inline bool IS_SUCCESS(int nRet)
		{
			return (nRet == yy_success);
		}
	}
}

#endif //_DW_PLATFORM_API_ERROR_CODES_H_
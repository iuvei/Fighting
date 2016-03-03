#ifndef CODEMSG_H
#define CODEMSG_H

#include <string>

using namespace std;

/*// 声明一个PackageCode
#ifndef DECLARE_PACK_CODE
#define DECLARE_PACK_CODE(code)\
	(code),
#endif */

// 定义一个PackageCode及其表示的意思
#ifndef DEFINE_PACK_CODE
#define DEFINE_PACK_CODE(code,msg)\
	 {code,(msg)}
#endif 


// 控制包类型（可以根据需要往里面添加自己的类型）
enum PackageCode
{
    code_unknown,								// 未知
    code_login,										// 登陆
    code_login_ret_ok,							// 登陆成功
    code_login_ret_failed,						// 登陆失败
    code_leave,										// 离开
    code_leave_ret,									// 离开（回应，这里可以不用回应）
    code_create_game,							// 创建游戏
    code_create_game_ret_ok,				// 创建游戏成功
    code_create_game_ret_failed,			// 创建游戏失败
    code_join_game_req_gameinfo,			// 客户端向服务器请求游戏信息
	code_join_game_rsp_gameinfo,			// 服务器向客户端返回游戏信息
	code_join_game,								// 加入游戏
    code_join_game_ret_ok,					// 加入游戏成功
    code_join_game_ret_failed,				// 加入游戏失败
    code_begin_game,							// 开始游戏
    code_begin_game_ret_ok,					//
    code_begin_game_ret_failed,
	code_suspend_game,
	code_suspend_game_ret_ok,
	code_suspend_game_ret_failed,
	code_continue_game,
	code_continue_game_ret_ok,
	code_continue_game_ret_failed,
    code_stop_game,
    code_stop_game_ret,
    code_select_hero,
    code_select_hero_ret_ok,
    code_select_hero_ret_failed,
};

struct CodeMsg
{
    PackageCode code;
    string      msg;
};

extern CodeMsg G_CodeMsgMap[];

string CodeToMsg(PackageCode code);

#endif // CODEMSG_H

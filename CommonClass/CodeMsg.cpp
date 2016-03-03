#include "CodeMsg.h"

CodeMsg G_CodeMsgMap[] =
{
	DEFINE_PACK_CODE(code_unknown,"unknown"),
	DEFINE_PACK_CODE(code_login,"login"),
	DEFINE_PACK_CODE(code_login_ret_ok,"login ok"),
	DEFINE_PACK_CODE(code_login_ret_failed,"login failed"),
	DEFINE_PACK_CODE(code_leave,"leave"),
	DEFINE_PACK_CODE(code_leave_ret,"leave return"),
	DEFINE_PACK_CODE(code_create_game,"create game"),
	DEFINE_PACK_CODE(code_create_game_ret_ok,"create game ok"),
	DEFINE_PACK_CODE(code_create_game_ret_failed,"create game failed"),
	DEFINE_PACK_CODE(code_join_game_req_gameinfo,"request game info"),
	DEFINE_PACK_CODE(code_join_game_rsp_gameinfo,"respond game info"),
	DEFINE_PACK_CODE(code_join_game,"join game"),
	DEFINE_PACK_CODE(code_join_game_ret_ok,"join game ok"),
	DEFINE_PACK_CODE(code_create_game_ret_failed,"join game failed"),
	DEFINE_PACK_CODE(code_begin_game,"begin game"),
	DEFINE_PACK_CODE(code_begin_game_ret_ok,"begin game ok"),
	DEFINE_PACK_CODE(code_begin_game_ret_failed,"begin game failed"),
	DEFINE_PACK_CODE(code_stop_game,"stop game"),
	DEFINE_PACK_CODE(code_stop_game_ret,"stop game return"),
	DEFINE_PACK_CODE(code_select_hero,"select game"),
	DEFINE_PACK_CODE(code_select_hero_ret_ok,"select game ok"),
	DEFINE_PACK_CODE(code_select_hero_ret_failed,"select game failed"),

	 /* {code_unknown,"unknown"},
    {code_login,"login"},
    {code_login_ret_ok,"login ok"},
    {code_login_ret_failed,"login failed"},
    {code_leave,"leave"},
    {code_leave_ret,"leave return"},
    {code_create_game,"create game"},
    {code_create_game_ret_ok,"create game ok"},
    {code_create_game_ret_failed,"create game failed"},
	{code_join_game_req_gameinfo,"request game info"},
	{code_join_game_rsp_gameinfo,"respond game info"},
    {code_join_game,"join game"},
    {code_join_game_ret_ok,"join game ok"},
    {code_create_game_ret_failed,"join game failed"},
    {code_begin_game,"begin game"},
    {code_begin_game_ret_ok,"begin game ok"},
    {code_begin_game_ret_failed,"begin game failed"},
    {code_stop_game,"stop game"},
    {code_stop_game_ret,"stop game return"},
    {code_select_hero,"select game"},
    {code_select_hero_ret_ok,"select game ok"},
    {code_select_hero_ret_failed,"select game failed"},*/

};


string CodeToMsg(PackageCode code)
{
    int count = sizeof(G_CodeMsgMap) / sizeof(CodeMsg);

    for(int i = 0; i < count; ++i)
    {
        if(code == G_CodeMsgMap[i].code)
        {
            return G_CodeMsgMap[i].msg;
            break;
        }
    }

    return G_CodeMsgMap[0].msg;
}

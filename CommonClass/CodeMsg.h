#ifndef CODEMSG_H
#define CODEMSG_H

#include <string>

using namespace std;

/*// ����һ��PackageCode
#ifndef DECLARE_PACK_CODE
#define DECLARE_PACK_CODE(code)\
	(code),
#endif */

// ����һ��PackageCode�����ʾ����˼
#ifndef DEFINE_PACK_CODE
#define DEFINE_PACK_CODE(code,msg)\
	 {code,(msg)}
#endif 


// ���ư����ͣ����Ը�����Ҫ����������Լ������ͣ�
enum PackageCode
{
    code_unknown,								// δ֪
    code_login,										// ��½
    code_login_ret_ok,							// ��½�ɹ�
    code_login_ret_failed,						// ��½ʧ��
    code_leave,										// �뿪
    code_leave_ret,									// �뿪����Ӧ��������Բ��û�Ӧ��
    code_create_game,							// ������Ϸ
    code_create_game_ret_ok,				// ������Ϸ�ɹ�
    code_create_game_ret_failed,			// ������Ϸʧ��
    code_join_game_req_gameinfo,			// �ͻ����������������Ϸ��Ϣ
	code_join_game_rsp_gameinfo,			// ��������ͻ��˷�����Ϸ��Ϣ
	code_join_game,								// ������Ϸ
    code_join_game_ret_ok,					// ������Ϸ�ɹ�
    code_join_game_ret_failed,				// ������Ϸʧ��
    code_begin_game,							// ��ʼ��Ϸ
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

#if (defined(WIN32) || defined(WIN64))
#include "../CommonClass/common.h"
#else
#include "../CommonClass/common.h"
#endif


using namespace std;

#include "FightingServer.h"

int main(int argc, char *argv[])
{
	FightingServer ss;
	ss.CreateServer(0,8899);
	ss.ServerLoop();

	return 0;
}

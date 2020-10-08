#if defined _inc_impl
	#undef _inc_impl
#endif

#include <YSI_Coding\y_hooks>

hook OnGameModeInit()
{
    systemlog = CreateLog("system");
    return 1;
}


hook OnGameModeExit()
{
    DestroyLog(systemlog);
    return 1;
}
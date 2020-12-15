#if defined _inc_impl
	#undef _inc_impl
#endif

#include <YSI_Coding\y_hooks>

hook OnPlayerInit(playerid)
{
	SetPlayerSkin(playerid, Player.Status[playerid][@skin]);
}
#if defined _inc_impl
	#undef _inc_impl
#endif

#include <YSI_Coding\y_hooks>

hook OnGroupInit()
{
	GROUP_ADD<LoggedGroup>
	{
		@YCMD:lock;
	}
	return 1;
}

YCMD:lock(playerid, params[], help)
{
	if (help) return SendClientMessage(playerid, -1, "not support!");

	call OnPlayerLock(playerid);

	return 1;
}

forward OnPlayerLock(playerid);
public OnPlayerLock(playerid)
{
	SendClientMessage(playerid, X11_GRAY71, "ไม่มีอะไรรอบ ๆ ตัวคุณที่สามารถล็อกได้ ...");
}
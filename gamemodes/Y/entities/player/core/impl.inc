#if defined _inc_impl
	#undef _inc_impl
#endif

#include <YSI_Coding\y_hooks>

hook OnGameModeInit()
{
	DeveloperGroup = Group_Create("Developer");
	return 1;
}

hook OnGroupInit()
{
	GROUP_ADD<LoggedGroup>
	{
		@YCMD:enter;
		@YCMD:exit;
	}
	return 1;
}

YCMD:enter(playerid, params[], help)
{
	if(help) return va_SendClientMessage(playerid, -1, "Not supported");
	call OnPlayerEnter(playerid);	
	return 1;
}

YCMD:exit(playerid, params[], help)
{
	if(help) return va_SendClientMessage(playerid, -1, "Not supported");
	call OnPlayerExit(playerid);	
	return 1;
}

forward OnPlayerEnter(playerid);
public OnPlayerEnter(playerid)
{
	SendClientMessage(playerid, X11_GRAY71, "   คุณไม่ได้อยู่ใกล้ทางเข้าใด ๆ");
	return 1;
}

forward OnPlayerExit(playerid);
public OnPlayerExit(playerid)
{
	SendClientMessage(playerid, X11_GRAY71, "   คุณไม่ได้อยู่ใกล้ทางออกใด ๆ");
	return 1;
}
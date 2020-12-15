#if defined _inc_impl
	#undef _inc_impl
#endif

#include <YSI_Coding\y_hooks>

hook OnGroupInit()
{
	GROUP_ADD<DeveloperGroup>
	{
		@YCMD:goint;
	}
	return 1;
}

YCMD:goint(playerid, params[], help) 
{
	if (help) return SendClientMessage(playerid, X11_WHITE, "ไม่รองรับ");

	new playa;
	if (sscanf(params, "d", playa)) {
		return 1;
	}

	if (playa < 0 || playa > 145) {
		return 1;
	}

	new string[64];
	format(string, sizeof(string), "Int %d: %s", playa, IntName[playa]);
	SendClientMessage(playerid, 0xFFFFFFAA, string);
	SetPlayerInterior(playerid, IntArray2[playa][0]);
	SetPlayerPos(playerid, IntArray[playa][0], IntArray[playa][1], IntArray[playa][2]);
	SetPlayerFacingAngle(playerid, IntArray[playa][3]);
	return 1;
}
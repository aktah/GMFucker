#if defined _inc_impl
	#undef _inc_impl
#endif

#include <YSI_Coding\y_hooks>

public OnPlayerEnterCheckpoint(playerid)
{
	// PlayerPlaySound(playerid, 1138, 0.0, 0.0, 0.0);
  	DisablePlayerCheckpoint(playerid);
	return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
  	DisablePlayerRaceCheckpoint(playerid);
	return 1;
}
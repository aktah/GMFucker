#if defined _inc_impl
	#undef _inc_impl
#endif

timer TimedKick[100](playerid)
{
	return Kick(playerid);
}

stock randomString(dest[], len)
{
	while(len--) {
		dest[len] = random(2) ? (random(26) + (random(2) ? 'a' : 'A')) : (random(10) + '0');
	}
}

stock bool:isFlagged(flags, flagValue) {
    if ((flags & flagValue) == flagValue) {
        return true;
    }
    return false;
}

stock IsPlayerNearPlayer(playerid, targetid, Float:radius)
{
	static
		Float:fX,
		Float:fY,
		Float:fZ
	;
	GetPlayerPos(targetid, fX, fY, fZ);
	return (GetPlayerInterior(playerid) == GetPlayerInterior(targetid) && GetPlayerVirtualWorld(playerid) == GetPlayerVirtualWorld(targetid)) && IsPlayerInRangeOfPoint(playerid, radius, fX, fY, fZ);
}

stock RemovePlayerWeaponID(playerid, weaponid)
{
	if(weaponid < 0 || weaponid > 50)
	    return;

	new 
		tempWeapon[13], 
		tempAmmo[13],
		currentWeapon = GetPlayerWeapon(playerid)
	;

	for(new slot = 0; slot < 13; slot++)
	    GetPlayerWeaponData(playerid, slot, tempWeapon[slot], tempAmmo[slot]);

	ResetPlayerWeapons(playerid);

	for(new slot; slot < 13; slot++)
	{
		if(tempWeapon[slot] == weaponid || tempAmmo[slot] <= 0)
			continue;

		GivePlayerWeapon(playerid, tempWeapon[slot], tempAmmo[slot]);
	}

	SetPlayerArmedWeapon(playerid, currentWeapon);
}

stock ClearChatBox(playerid, rows = 20) for (new i = 0; i < rows; i ++) SendClientMessage(playerid, -1, "");
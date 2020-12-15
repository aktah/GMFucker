#if defined _inc_impl
	#undef _inc_impl
#endif

#include <YSI_Coding\y_hooks>

hook OnDatabaseInit() 
{

    inline Handler()
    {
		new
			rows = cache_num_rows(),
			idx = INVALID_ITERATOR_SLOT
		;

		for (new row = 0; row != rows; ++row) {

			new id;
			cache_get_value_name_int(row, "id", id); // 1

		    idx = Iter_Free(IRentcar);

		    if (idx == INVALID_ITERATOR_SLOT) {
		    	err("Number of rentcar (%d) is greater than the limit (%d)", rows, Y_FK_MAX_RENTCAR);
		    	break;
		    }

		    Rentcar.Meta[idx][@id] = DBID:id;

			cache_get_value_name_int(row, "modelid", Rentcar.Meta[idx][@modelid]);
			cache_get_value_name_int(row, "price", Rentcar.Meta[idx][@price]);
		
			cache_get_value_name_float(row, "x", Rentcar.Meta[idx][@x]);
			cache_get_value_name_float(row, "y", Rentcar.Meta[idx][@y]);
			cache_get_value_name_float(row, "z", Rentcar.Meta[idx][@z]);
			cache_get_value_name_float(row, "a", Rentcar.Meta[idx][@a]);

			Rentcar.Meta[idx][@player] = INVALID_PLAYER_ID;

			Rentcar.Meta[idx][@car] = CreateCoreVehicle(Rentcar.Meta[idx][@modelid], Rentcar.Meta[idx][@x], Rentcar.Meta[idx][@y], Rentcar.Meta[idx][@z], Rentcar.Meta[idx][@a], .plate = "รถเช่า", .lockState = false);

		    Iter_Add(IRentcar, idx);
		}

		printf("Loaded %d Car rental", Iter_Count(IRentcar));
	}
	MySQL_PQueryInline(dbHandle, using inline Handler, "SELECT * FROM " Y_FK_TABLE_NAME "");
	return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
	new rent = RentCar_PlayerRental(playerid);
	if (rent != -1) {

		new vehicleid = Rentcar.Meta[rent][@car];
		SetVehicleToRespawn(vehicleid);

		new 
			engine, 
			lights, 
			alarm, 
			doors, 
			bonnet, 
			boot, 
			objective
		;

		Vehicle.Meta[vehicleid][@engine] = false;
		Vehicle.Meta[vehicleid][@light] = false;
		Vehicle.Meta[vehicleid][@lock] = false;

		GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
		SetVehicleParamsEx(vehicleid, Vehicle.Meta[vehicleid][@engine], Vehicle.Meta[vehicleid][@light], alarm, Vehicle.Meta[vehicleid][@lock], bonnet, boot, objective);

		Rentcar.Meta[rent][@player] = INVALID_PLAYER_ID;
	}
	return 1;
}

hook OnGroupInit()
{
	GROUP_ADD<DeveloperGroup>
	{
		@YCMD:createrentcar;
	}

	GROUP_ADD<LoggedGroup>
	{
		@YCMD:rentcar;
		@YCMD:unrentcar;
	}

	return 1;
}

RentCar_GetID(vehicleid)
{
	foreach(new rent:IRentcar)
	{
		if (Rentcar.Meta[rent][@car] == vehicleid)
		{
			return rent;
		}
	}
	return -1;
}

RentCar_PlayerRental(playerid)
{
	foreach(new rent:IRentcar)
	{
		if (Rentcar.Meta[rent][@player] == playerid)
		{
			return rent;
		}
	}
	return -1;
}

stock bool:IsRentCar(vehicleid)
{
	foreach(new rent:IRentcar)
	{
		if (Rentcar.Meta[rent][@car] == vehicleid)
		{
			return true;
		}
	}
	return false;
}

hook OnPlayerStateChange(playerid, newstate, oldstate)
{
	if (newstate == PLAYER_STATE_DRIVER) {
		new vehicleid = GetPlayerVehicleID(playerid);
		new rent = RentCar_GetID(vehicleid);

		if (rent == INVALID_ITERATOR_SLOT)
			return 1;

		va_SendClientMessage(playerid, X11_SPRINGGREEN4, "รถเช่า %s ราคา $%d", ReturnVehicleNameByModel(Vehicle.Meta[vehicleid][@modelid]), Rentcar.Meta[rent][@price]);
		SendClientMessage(playerid, X11_SPRINGGREEN4, "พิมพ์ /rentcar เพื่อเช่า!");
	}
	return 1;
}

YCMD:createrentcar(playerid, params[], help)
{
	if (help) return SendClientMessage(playerid, -1, "not support!");


	return 1;
}

YCMD:unrentcar(playerid, params[], help)
{
	if (help) return SendClientMessage(playerid, -1, "not support!");

	new rent = RentCar_PlayerRental(playerid);
	if (rent == -1) return SendClientMessage(playerid, X11_INDIANRED, "คุณยังไม่ได้เช่ารถ!");

	new vehicleid = Rentcar.Meta[rent][@car];
	SetVehicleToRespawn(vehicleid);

	new 
		engine, 
		lights, 
		alarm, 
		doors, 
		bonnet, 
		boot, 
		objective
	;

	Vehicle.Meta[vehicleid][@engine] = false;
	Vehicle.Meta[vehicleid][@light] = false;
	Vehicle.Meta[vehicleid][@lock] = false;

	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	SetVehicleParamsEx(vehicleid, Vehicle.Meta[vehicleid][@engine], Vehicle.Meta[vehicleid][@light], alarm, Vehicle.Meta[vehicleid][@lock], bonnet, boot, objective);

	Rentcar.Meta[rent][@player] = INVALID_PLAYER_ID;
	SendClientMessage(playerid, X11_SPRINGGREEN4, "คุณได้เลิกเช่ารถยนต์เรียบร้อยแล้ว");
		
	return 1;
}

YCMD:rentcar(playerid, params[], help)
{
	if (help) return SendClientMessage(playerid, -1, "not support!");

	if (!IsPlayerInAnyVehicle(playerid)) 
		return SendClientMessage(playerid, X11_INDIANRED, "คุณไม่ได้อยู่บนรถเช่า!");

	if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
		return SendClientMessage(playerid, X11_INDIANRED, "คุณต้องเป็นคนขับรถ!");

	new 
		vehicleid = GetPlayerVehicleID(playerid),
		rent = RentCar_GetID(vehicleid)
	;

	if (rent == INVALID_ITERATOR_SLOT)
		return SendClientMessage(playerid, X11_INDIANRED, "คุณไม่ได้อยู่บนรถเช่า!");

	if (RentCar_PlayerRental(playerid) != -1)
		return SendClientMessage(playerid, X11_INDIANRED, "คุณได้เช่ารถไปแล้ว พิมพ์ /unrentcar เพื่อเลิกเช่า!");

	if (Player.Status[playerid][@cash] < Rentcar.Meta[rent][@price])
		return SendClientMessage(playerid, X11_INDIANRED, "คุณมีเงินไม่พอที่จะเช่ารถคันนี้!");

	Player.Status[playerid][@cash] -= Rentcar.Meta[rent][@price];
	Rentcar.Meta[rent][@player] = playerid;

	/*inline Handler()
	{
		Rentcar.Meta[rent][@player] = playerid;

		va_SendClientMessage(playerid, X11_SPRINGGREEN4, "คุณได้เช่ารถ %s ในราคา $%d", ReturnVehicleNameByModel(Vehicle.Meta[vehicleid][@modelid]), Rentcar.Meta[rent][@price]);
		
		new 
			engine, 
			lights, 
			alarm, 
			doors, 
			bonnet, 
			boot, 
			objective
		;
		Vehicle.Meta[vehicleid][@engine] = true;
		GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
		SetVehicleParamsEx(vehicleid, Vehicle.Meta[vehicleid][@engine], lights, alarm, doors, bonnet, boot, objective);
		@return 1;
	}

	new query[128];
	format(query, sizeof query, "UPDATE " CHARACTER_STATUS_TABLE " SET cash = %d WHERE character_id = %d", Player.Status[playerid][@cash], _:Player.Status[playerid][@id]);
	MySQL_TQueryInline(dbHandle, using inline Handler, query);*/

	new query[128];
	format(query, sizeof query, "UPDATE " CHARACTER_STATUS_TABLE " SET cash = %d WHERE character_id = %d", Player.Status[playerid][@cash], _:Player.Status[playerid][@id]);
	mysql_tquery(dbHandle, query);
	return 1;
}

hook OnPlayerLock(playerid)
{
	new rent = RentCar_PlayerRental(playerid);

	if (rent == -1)
		return 1;

	new vehicleid = GetNearestVehicle(playerid);
	if (vehicleid == INVALID_VEHICLE_ID || vehicleid != Rentcar.Meta[rent][@car])
		return 1;

	new 
		engine, 
		lights, 
		alarm, 
		doors, 
		bonnet, 
		boot, 
		objective
	;

	Vehicle.Meta[vehicleid][@lock] = !Vehicle.Meta[vehicleid][@lock];
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	SetVehicleParamsEx(vehicleid, engine, lights, alarm, Vehicle.Meta[vehicleid][@lock], bonnet, boot, objective);

	new string[64], Float:vPosX, Float:vPosY, Float:vPosZ;
	GetVehiclePos(vehicleid, vPosX, vPosY, vPosZ);

	format(string, sizeof string, "~%s~%s %sLOCKED", Vehicle.Meta[vehicleid][@lock] ? ("r") : ("g"), ReturnVehicleNameByModel(Vehicle.Meta[vehicleid][@modelid]), Vehicle.Meta[vehicleid][@lock] ? ("") : ("UN"));
	GameTextForPlayer(playerid, string, 2000, 3);
	
	PlayerPlaySound(playerid, 24600, vPosX, vPosY, vPosZ);
	return Y_HOOKS_BREAK_RETURN_1;
}
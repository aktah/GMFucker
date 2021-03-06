#if defined _inc_impl
	#undef _inc_impl
#endif

#include <YSI_Coding\y_hooks>

hook OnGameModeInit()
{
	CreateAdminGroups();
	Command_AddAlt(YCMD:setadminlevel, "sal");
	Command_AddAlt(YCMD:adminhelp, "ahelp");
}

hook OnGroupInit()
{
	Group_AddChild(AdminGroup[0], DeveloperGroup);

	GROUP_ADD<AdminGroup[0]>
	{
		@YCMD:adminhelp;
		@YCMD:ahelp;
	}

	GROUP_ADD<AdminGroup[Y_PLAYER_ADMIN_MAX_LEVEL - 1]>
	{
		@YCMD:setadminlevel;
		@YCMD:sal;
	}

	GROUP_ADD<AdminGroup[Y_PLAYER_ADMIN_MANAGE_LEVEL - 1]>
	{
		@YCMD:spawncar;
		@YCMD:despawncar;
	}

	return 1;
}

// http://forum.sa-mp.com/showpost.php?p=2894606&postcount=3
hook OnRconLoginAttempt(ip[], password[], success)
{
	new ip2 = IPToInt(ip);

	if (success) {
		defer RetrieveRconPlayer(ip2);
	}

	return 1;
}

hook OnPlayerRconLogin(playerid)
{
	SetAdminLevel(playerid, Y_PLAYER_ADMIN_MAX_LEVEL, true);
	return 1;
}

timer RetrieveRconPlayer[1000](ip)

{
	foreach(new player:Player) {
		if (Player.Status[player][@ip] == ip && IsPlayerAdmin(player)) {
			CallRemoteFunction("OnPlayerRconLogin", "i", player);
			return 1;
		}
	}

	return 1;
}

CreateAdminGroups()
{
	for (new i = 0; i != Y_PLAYER_ADMIN_MAX_LEVEL; ++i) {
		new name[32];
		format(name, sizeof name, "Admin level %d", i + 1);
		AdminGroup[i] = Group_Create(name);

		if (i > 0) {
			Group_AddChild(AdminGroup[i], AdminGroup[i - 1]);
		}
	}
}

ClearAdminLevel(playerid)
{
	if (Player.Status[playerid][@admin] > 0) {
		Group_SetPlayer(AdminGroup[Player.Status[playerid][@admin] - 1], playerid, false);	
	}
}

SetAdminLevel(playerid, level, bool:force = false)
{
	if (!force && IsPlayerAdmin(playerid)) {
		return 2;
	}

	ClearAdminLevel(playerid);

	if (!(1 <= level <= Y_PLAYER_ADMIN_MAX_LEVEL)) {
		log("player %d admin level cannot be set to %d", playerid, level);
		return 1;
	}

	Group_SetPlayer(AdminGroup[level - 1], playerid, true);
	Player.Status[playerid][@admin] = level;
	return 0;
}

SendAdminMessage(E_ADMIN_MSG:type, level, const fmat[],  va_args<>)
{
	if (level < 1 || level > Y_PLAYER_ADMIN_MAX_LEVEL) {
		err("can't send message to admin level %d", level);
		return;
	}

	new
		str[145],
		color = X11_YELLOW2
	;

	va_format(str, sizeof (str), fmat, va_start<3>);

	switch (type) {
		case ADMIN_MSG_INFO: {
			color = X11_YELLOW3;
			format(str, sizeof str, "AdmInfo: %s", str);
		}
		case ADMIN_MSG_WARNING: {
			color = X11_YELLOW1;
			format(str, sizeof str, "AdmWarning: %s", str);
		}
	}

	foreach(new player:GroupMember(AdminGroup[level - 1]))
	{
		SendClientMessage(player, color, str);
	}
	return;
}

IsAdminLevel(playerid, level)
{
	if (IsPlayerAdmin(playerid)) {
		return true;
	}

	if (1 <= level <= Y_PLAYER_ADMIN_MAX_LEVEL) {
		return Group_GetPlayer(AdminGroup[level - 1], playerid);
	}

	return false;
}

YCMD:setadminlevel(playerid, params[], help)
{
	if(help) return SendClientMessage(playerid, -1, "Not supported");

	new
		targetid,
		level
	;

	if (sscanf(params, "ud", targetid, level)) return SendClientMessage(playerid, X11_LIGHTBLUE, "�����"WHITE" /sal <�ʹ�/���ͺҧ��ǹ> <�����>");
	if (targetid == playerid) return SendClientMessage(playerid, X11_INDIANRED, "�س�������ö���á�зӹ��Ѻ����ͧ��");
	if (!IsPlayerConnected(targetid)) return SendClientMessage(playerid, X11_INDIANRED, "��辺�����蹷���к�");
	if (Player.Status[playerid][@admin] <= Player.Status[targetid][@admin]) return SendClientMessage(playerid, X11_INDIANRED, "ACCESS DENIED: "WHITE"�س������Ѻ͹حҵ");

	new sal = SetAdminLevel(targetid, level);
	if(sal) {
		if (sal == 1) return va_SendClientMessage(playerid, X11_INDIANRED, "�к���������١��ͧ ����ŷ���������������ҧ 1 ��� %d", Y_PLAYER_ADMIN_MAX_LEVEL);
		else if (sal == 2) return va_SendClientMessage(playerid, X11_INDIANRED, "ACCESS DENIED: "WHITE"�س������Ѻ͹حҵ");
	}

	SendClientMessage(playerid, X11_KHAKI, "���Թ������������");
	foreach(new player:GroupMember(AdminGroup[0])) {
		va_SendClientMessage(player, X11_LIGHTBLUE, "������ʹ�Թ�ͧ %s �١����¹������� %d ���ʹ�Թ %s", Player.Auth[targetid][@name], level, Player.Auth[playerid][@name]);
	}

	new query[80];
	mysql_format(dbHandle, query, sizeof query, "UPDATE " CHARACTER_STATUS_TABLE " SET admin = %d WHERE character_id = %d", Player.Status[targetid][@admin], _:Player.Status[targetid][@id]);
	mysql_pquery(dbHandle, query);
	return 1;
}

YCMD:spawncar(playerid, params[], help)
{
	if(help) return SendClientMessage(playerid, -1, "Not supported");

	new 
		slot = Iter_Free(IAdminVehicle), 
		modelid, 
		color1, 
		color2, 
		siren
	; 
	
	if (sscanf(params, "iI(-1)I(-1)I(0)", modelid, color1, color2, siren))
	{
		SendClientMessage(playerid, X11_LIGHTBLUE, "�����"WHITE" /spawncar <�ʹ�����> <��1> <��2> <��ù ���������� 0>");
		SendClientMessage(playerid, X11_GRAY71, "������ҹ��˹Ъ��Ǥ��� ��ù��͡��͹حҵ���س�Դ��ù��������ö¹��"); 
		return 1;
	}
	
	if (modelid < 400 || modelid > 611)
		return SendClientMessage(playerid, X11_INDIANRED, "�س�к����ŷ�����١��ͧ");
		
	if (color1 < -1 || color2 < -1 || color1 > 255 || color2 > 255)
		return SendClientMessage(playerid, X11_INDIANRED, "�շ��س�к����١��ͧ"); 
	
	if (slot == INVALID_ITERATOR_SLOT)
		return SendClientMessage(playerid, X11_INDIANRED, "�������ö���ҧ�ҹ��˹����ҡ���ҹ������"); 

	new Float:pX, Float:pY, Float:pZ;
	GetPlayerPos(playerid, pX, pY, pZ);

	new vehicleid = CreateCoreVehicle(modelid, pX, pY, pZ, 0, color1, color2, -1, siren, .engineState = true, .lightState = true, .lockState = false, .plate = "ADMIN VEHICLE");
	
	if(vehicleid > 0 && vehicleid != INVALID_VEHICLE_ID) {
		Iter_Add(IAdminVehicle, vehicleid);
		PutPlayerInVehicle(playerid, vehicleid, 0);
		SendAdminMessage(ADMIN_MSG_INFO, 1, "%s ���ҧ�ҹ��˹� %s (%d)", Player.Status[playerid][@name], ReturnVehicleNameByModel(Vehicle.Meta[vehicleid][@modelid]), vehicleid);
	} else {
		err("can't create vehicle model %d", modelid);
	}
	return 1;
}

YCMD:despawncar(playerid, params[], help)
{
	if(help) return SendClientMessage(playerid, -1, "Not supported");

	new vehicleid; 
	
	if (sscanf(params, "i", vehicleid)) return SendClientMessage(playerid, X11_LIGHTBLUE, "�����"WHITE" /despawncar <�ʹ��ҹ��˹�> (/dl)");

	if (!Iter_Contains(IAdminVehicle, vehicleid))	return SendClientMessage(playerid, X11_INDIANRED, "�ҹ��˹Фѹ���������Ҩҡ�ʹ�Թ");

	Iter_Remove(IAdminVehicle, vehicleid);
	DestroyCoreVehicle(vehicleid);
	return 1;
}

YCMD:adminhelp(playerid, params[], help)
{
	if(help) return SendClientMessage(playerid, -1, "Not supported");

	if (IsAdminLevel(playerid, 1)) {
		SendClientMessage(playerid, X11_SPRINGGREEN4,"__________������ʹ�Թ__________");
		SendClientMessage(playerid, X11_SKYBLUE,"/ahelp /aduty");
	}

	if (IsAdminLevel(playerid, Y_PLAYER_ADMIN_MANAGE_LEVEL)) {
		SendClientMessage(playerid, X11_SKYBLUE,"/gopos /createdoor /doors /spawncar /despawncar");
		SendClientMessage(playerid, X11_SKYBLUE, "/makehouse /gotohouse /gotohouseint");
		SendClientMessage(playerid, X11_SKYBLUE, "/createfaction /changefowner");
	}

	if (IsAdminLevel(playerid, Y_PLAYER_ADMIN_MAX_LEVEL)) {
		SendClientMessage(playerid, X11_SKYBLUE,"/sal");
	}
	return 1;
}

/**
#define Y_PLAYER_ADMIN_MAX_LEVEL        5
#define Y_PLAYER_ADMIN_MANAGE_LEVEL     4
#define Y_PLAYER_ADMIN_LEAD_LEVEL       3
#define Y_PLAYER_ADMIN_STAFF_LEVEL      2
 */
#if defined _inc_impl
	#undef _inc_impl
#endif

#include <YSI_Coding\y_hooks>

static 
	Timer:inactivityKick[MAX_PLAYERS],
	attempt[MAX_PLAYERS]
;

hook OnPlayerDisconnect(playerid, reason)
{
	stop inactivityKick[playerid];
	attempt[playerid] = 0;
	return 1;
}

hook OnPlayerRequestClass(playerid, classid)
{
	defer LoginCamera(playerid);
	return 1;
}

timer KickOnInactivity[30000](playerid)
{
	SendClientMessage(playerid, X11_INDIANRED, "[ ! ] "WHITE"�س�١�����ͧ�ҡ������������к�");
	defer TimedKick(playerid);
}

timer LoginNextAttempt[500](playerid)
{
	ShowLoginDialog(playerid);
}

ShowLoginDialog(playerid)
{
	new
		error[32]
	;

	inactivityKick[playerid] = repeat KickOnInactivity[Y_PLAYER_LOGIN_TIMEOUT_KICK * 1000](playerid);

	if (attempt[playerid] > 0) {
		format(error, sizeof error, ""INDIANRED"���ʼ�ҹ���١��ͧ\n\n");
	}

	if (attempt[playerid] >= Y_PLAYER_LOGIN_MAX_ATTEMPTS) {
		SendClientMessage(playerid, X11_INDIANRED, "ERROR: "WHITE"�س��͡���ʼ�ҹ�Դ�ҡ�Թ�");
		defer TimedKick(playerid);
		return 1;
	}

	inline HandleLogin(pid, dialogid, response, listitem, string:inputtext[])
	{
		#pragma unused pid, dialogid, listitem

		stop inactivityKick[playerid];

		if (!response) {
			defer TimedKick(playerid);
			@return 1;
		}

		inline const OnChecked(bool:success)
		{
			if(success) {
				SelectCharacter(playerid);
			} else {
				attempt[playerid]++;
				defer LoginNextAttempt[500](playerid);
			}
		}

		BCrypt_CheckInline(inputtext, Player.Auth[playerid][@password], using inline OnChecked);
		@return 1;
	}

	new body[256];

	if (attempt[playerid] > 0) {
		format(body, sizeof body, "%s"WHITE"�ô��͡���ʼ�ҹ�ͧ�س��ҹ��ҧ��������������к�:\n\n"FIREBRICK"�����������㹡���������к� (%d/%d)", error, attempt, Y_PLAYER_LOGIN_MAX_ATTEMPTS);
	}
	else {
		format(body, sizeof body, "%s"WHITE"����������к� "SERVER_COL"["WEBSITE_URL"]\n\n"WHITE"�ô��͡���ʼ�ҹ��ҹ��ҧ�������͡����Фâͧ��ҹ\n- ����¹ʡԹ����Ф�����˹�����䫵� "SERVER_COL"(UCP)", error);
	}

	Dialog_ShowCallback(playerid, using inline HandleLogin, DIALOG_STYLE_PASSWORD, "�Թ�յ�͹�Ѻ������ "SERVER_NAME"", body, "�������к�", "�͡");
	return 1;
}

timer LoginCamera[100](playerid)
{
	switch(random(4))
	{
		case 0:
		{
			SetPlayerPos(playerid, 290.4137,-1555.2571,-15.9629);
			SetPlayerCameraPos(playerid, 189.4343,-1600.7451,74.6708);
			SetPlayerCameraLookAt(playerid, 290.4137,-1555.2571,67.9629);
		}
		case 1:
		{
			SetPlayerPos(playerid, 1044.0846,-1621.3650,-38.9547);
			SetPlayerCameraPos(playerid, 1044.0846,-1621.3650,108.2995);
			SetPlayerCameraLookAt(playerid, 1154.1254,-1770.6908,16.5938);
		}
		case 2:
		{
			SetPlayerPos(playerid, 2170.4578,-1674.4376,-78.3328);
			SetPlayerCameraPos(playerid, 2161.3362,-1687.6860,143.4194);
			SetPlayerCameraLookAt(playerid, 2537.1755,-1705.1808,13.3828);
		}
		case 3: {
			SetPlayerPos(playerid, 2091.1182,-1849.2242,8.5529);
			SetPlayerCameraPos(playerid, 2091.1182,-1849.2242,27.5529);
			SetPlayerCameraLookAt(playerid, 2070.0266,-1813.8236,13.3828);
		}
	}
}

hook OnPlayerLogin(playerid)
{
	ClearPlayerMeta(playerid);
	return 1;
}

ClearPlayerMeta(playerid)
{
	memset(Player.Auth[playerid][@password], 0);
	memset(Player.Status[playerid][@secretkey], 0);
}
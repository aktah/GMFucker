#if defined _inc_impl
	#undef _inc_impl
#endif

#include <YSI_Coding\y_hooks>

hook OnPlayerRegister(playerid)
{
	#if defined Y_PLAYER_LOGIN_AFTER_REG
	SelectCharacter(playerid);
	#else
	FetchPlayer(playerid);
	#endif
	
	return 1;
}

ShowRegistrationDialog(playerid, e_ERROR_PASSWORD:errorCode = e_ERROR_PASSWORD_NONE)
{
	new
		error[64]
	;

	if (errorCode != e_ERROR_PASSWORD_NONE) {
		switch (errorCode) {
			case e_ERROR_PASSWORD_EMPTY: {
				format(error, sizeof error, ""INDIANRED"���ʼ�ҹ��ͧ����繤����ҧ\n\n");
			}
			case e_ERROR_PASSWORD_SHORT: {
				format(error, sizeof error, ""INDIANRED"���ʼ�ҹ��ͧ�դ���������ҧ���� %d ����ѡ��\n\n", Y_PLAYER_REGISTRATION_MIN_PASS_LENGTH);
			}
			case e_ERROR_PASSWORD_LONG: {
				format(error, sizeof error, ""INDIANRED"���ʼ�ҹ�դ�������٧�ش�� %d ����ѡ��\n\n", Y_PLAYER_REGISTRATION_MAX_PASS_LENGTH);
			}
		}
	}

	inline HandleRegistration(pid, dialogid, response, listitem, string:inputtext[])
	{
		#pragma unused pid, dialogid, listitem

		if (!response) {
			defer TimedKick(playerid);
			@return 1;
		}

		@return CheckRegistrationPassword(playerid, inputtext);
	}

	new body[190];
	format(body, sizeof body, "%s"WHITE"�Թ�յ�͹�Ѻ����������!\n��͡���ʼ�ҹ����ͧ��ô�ҹ��ҧ������\n���ʼ�ҹ��ͧ�դ�����ǵ���� %d �֧ %d ����ѡ��", error, Y_PLAYER_REGISTRATION_MIN_PASS_LENGTH, Y_PLAYER_REGISTRATION_MAX_PASS_LENGTH);
	Dialog_ShowCallback(playerid, using inline HandleRegistration, DIALOG_STYLE_PASSWORD, "�Թ�յ�͹�Ѻ������ "SERVER_NAME"", body, "��Ѥ���Ҫԡ", "�͡");
	return 1;
}

CheckRegistrationPassword(playerid, password[])
{
	new len = strlen(password);

	if (isnull(password)) {
		return ShowRegistrationDialog(playerid, e_ERROR_PASSWORD_EMPTY);
	}

	if (len < Y_PLAYER_REGISTRATION_MIN_PASS_LENGTH) {
		return ShowRegistrationDialog(playerid, e_ERROR_PASSWORD_SHORT);
	}

	if (len > Y_PLAYER_REGISTRATION_MAX_PASS_LENGTH) {
		return ShowRegistrationDialog(playerid, e_ERROR_PASSWORD_LONG);
	}

	return Register(playerid, password);
}

Register(playerid, const password[])
{
	inline const OnHashed(string:result[])
	{
		strcpy(Player.Auth[playerid][@password], result);

		new
			query[320]
		;
		mysql_format(dbHandle, query, sizeof query, "INSERT INTO " PLAYER_TABLE " (name, password) VALUES ('%e', '%e')", Player.Auth[playerid][@name], Player.Auth[playerid][@password]);
		
		inline SavePlayer()
		{
			Player.Auth[playerid][@id] = DBID:cache_insert_id();
			call OnPlayerRegister(playerid);
		}
		MySQL_TQueryInline(dbHandle, using inline SavePlayer, query);
	}
	BCrypt_HashInline(password, Y_BCRYPT_WORK_FACTOR, using inline OnHashed);
	return 1;
}
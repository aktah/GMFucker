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
				format(error, sizeof error, ""INDIANRED"รหัสผ่านต้องไม่เป็นค่าว่าง\n\n");
			}
			case e_ERROR_PASSWORD_SHORT: {
				format(error, sizeof error, ""INDIANRED"รหัสผ่านต้องมีความยาวอย่างน้อย %d ตัวอักษร\n\n", Y_PLAYER_REGISTRATION_MIN_PASS_LENGTH);
			}
			case e_ERROR_PASSWORD_LONG: {
				format(error, sizeof error, ""INDIANRED"รหัสผ่านมีความยาวสูงสุดแค่ %d ตัวอักษร\n\n", Y_PLAYER_REGISTRATION_MAX_PASS_LENGTH);
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
	format(body, sizeof body, "%s"WHITE"ยินดีต้อนรับผู้เล่นใหม่!\nกรอกรหัสผ่านที่ต้องการด้านล่างนี้เลย\nรหัสผ่านต้องมีความยาวตั้งแต่ %d ถึง %d ตัวอักษร", error, Y_PLAYER_REGISTRATION_MIN_PASS_LENGTH, Y_PLAYER_REGISTRATION_MAX_PASS_LENGTH);
	Dialog_ShowCallback(playerid, using inline HandleRegistration, DIALOG_STYLE_PASSWORD, "ยินดีต้อนรับเข้าสู่ "SERVER_NAME"", body, "สมัครสมาชิก", "ออก");
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
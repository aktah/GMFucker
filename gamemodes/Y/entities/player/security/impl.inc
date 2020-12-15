#if defined _inc_impl
	#undef _inc_impl
#endif

#include <YSI_Coding\y_hooks>

forward OnSecretKeyHash(playerid);
public OnSecretKeyHash(playerid)
{
    new dest[250];
    bcrypt_get_hash(dest);

    inline UpdateSecretKey()
    {
        @return VerifyAccount(playerid);
    }

    new query[112];
    mysql_format(dbHandle, query, sizeof(query), "UPDATE characters SET secret_key = '%e' WHERE id = %d", dest, _:Player.Status[playerid][@id]); 
    MySQL_TQueryInline(dbHandle, using inline UpdateSecretKey, query);
}

forward OnSecretKeyVerify(playerid,bool:success);
public OnSecretKeyVerify(playerid,bool:success) {
 	if(success) {
        inline UpdatePlayerIP()
        {
            @return VerifyAccount(playerid);
        }
        new ip[18], query[100];
        GetPlayerIp(playerid, ip, sizeof ip);
        mysql_format(dbHandle, query, sizeof(query), "UPDATE characters SET ip = '%e', logged_at = NOW() WHERE id = %d", ip, _:Player.Status[playerid][@id]);
        MySQL_TQueryInline(dbHandle, using inline UpdatePlayerIP, query);
 	} else {
        SendClientMessage(playerid, X11_INDIANRED, "ACCESS DENIED: "WHITE"คุณถูกเตะเนื่องจากกรอกรหัสลับไม่ถูกต้อง สิ่งนี้มีไว้ด้วยเหตุผลด้านความปลอดภัย");
		defer TimedKick(playerid);
 	}
 }

ShowSecretCreationDialog(playerid)
{
    inline HandleCreationSecretWord(pid, dialogid, response, listitem, string:inputtext[])
    {
        #pragma unused pid, dialogid, listitem

        if(!response)
        {
            defer TimedKick(playerid);
            @return 0;
        }

        if (isnull(inputtext)) {
            @return ShowSecretCreationDialog(playerid);
        }

        bcrypt_hash(playerid, "OnSecretKeyHash", inputtext, Y_BCRYPT_WORK_FACTOR);
        @return 1;
    }
    Dialog_ShowCallback(playerid, using inline HandleCreationSecretWord, DIALOG_STYLE_PASSWORD, "รหัสความปลอดภัย", ""INDIANRED"ข้อควรระวังด้านความปลอดภัย:\n\n"WHITE"เราขอแนะนำระบบการยืนยันตัวตนด้วยรหัสลับเพื่อช่วยปกป้องบัญชีผู้ใช้\nสิ่งนี้จะถูกแสดงขึ้นมาเมื่อเชื่อมต่อของคุณมีการเปลี่ยนแปลงไป\n\n"WHITE"คุณจะต้องจำสิ่งนี้ไว้("FIREBRICK"ห้ามลืมเด็ดขาด!!!"WHITE")\n\n"FIREBRICK"ขอแจ้งให้ทราบว่านี่ไม่ใช่รหัสผ่านของคุณ", "ยืนยัน", "ออก");
	return 1;
}

ShowSecretVerifyDialog(playerid)
{
    inline HandleVerifySecretWord(pid, dialogid, response, listitem, string:inputtext[])
    {
        #pragma unused pid, dialogid, listitem

        if(!response)
        {
            defer TimedKick(playerid);
            @return 0;
        }

        if (isnull(inputtext)) {
            @return ShowSecretVerifyDialog(playerid);
        }

        bcrypt_verify(playerid,"OnSecretKeyVerify", inputtext, Player.Status[playerid][@secretkey]);
        
        @return 1;
    }
    Dialog_ShowCallback(playerid, using inline HandleVerifySecretWord, DIALOG_STYLE_PASSWORD, "รหัสความปลอดภัย", ""INDIANRED"ข้อควรระวังด้านความปลอดภัย:\n\n"WHITE"ระบบของเราได้ตั้งสถานะการเปลี่ยนแปลงการเชื่อมต่อบัญชีของคุณ\nเพื่อให้แน่ใจว่าไม่มีการละเมิดความปลอดภัย\n\n"WHITE"โปรดกรอก "FIREBRICK"รหัสลับ"WHITE" ที่คุณตั้งในระหว่างการลงทะเบียนเพื่อเข้าสู่ระบบ", "ยืนยัน", "ออก");
    return 1;
}


forward VerifyAccount(playerid);
public VerifyAccount(playerid)
{
    new query[160];

	inline HandleSecurityVerify()
	{
	    if (!cache_num_rows()) {
	    	@return err("can't load character data", playerid);
	    }

        new
            bool:active,
            loginIp[18],
            ip[18],
            day
        ;

        cache_get_value_name(0, "secret_key", Player.Status[playerid][@secretkey]);
        cache_get_value_name_bool(0, "active", active);
        cache_get_value_name(0, "ip", loginIp);
        cache_get_value_name_int(0, "day", day);

        GetPlayerIp(playerid, ip, sizeof ip);
    
        if (isnull(Player.Status[playerid][@secretkey])) {
            @return ShowSecretCreationDialog(playerid);
        }
        else if (!active) {
            format(query, sizeof(query), "UPDATE ucp_applications SET fingerprint = 1 WHERE character_id = %d", _:Player.Status[playerid][@id]);
            mysql_pquery(dbHandle, query);
            
            SendClientMessage(playerid, X11_INDIANRED, "NOTE: จนกว่าใบสมัครของคุณและตัวละครของคุณจะได้รับการยืนยัน คุณจะไม่สามารถเข้าเล่นได้");
            SendClientMessage(playerid, X11_INDIANRED, "VISIT: "WEBSITE_URL"");
            SendClientMessage(playerid, X11_INDIANRED, "ตัวละครของคุณยังไม่ได้รับการยืนยัน");
            SendClientMessage(playerid, X11_INDIANRED, "ด้วยความเคารพจากทีมงาน "SERVER_ALIAS"");

            defer TimedKick(playerid);
            @return 0;
        }
        else if (day >= Y_PLAYER_SECURE_LOGIN_DAY) {

			SendClientMessage(playerid, X11_YELLOW, "การยืนยันตนเองสองระดับ");
			SendClientMessage(playerid, X11_YELLOW, "บัญชีนี้ไม่ได้เข้าสู่ระบบในระยะเวลาที่นานพอสมควร");
			SendClientMessage(playerid, X11_YELLOW, "เพื่อให้บัญชีของคุณปลอดภัย เราต้องการให้คุณยืนยันความเป็นเจ้าของบัญชีของคุณ");

            format(query,sizeof(query),"ip=%s&time=%d&id=%d", loginIp, gettime(), _:Player.Status[playerid][@id]);
            HTTP(playerid, HTTP_POST, "localhost/api/verify_email.php", query,"HttpVerifyRequest");
            @return 0;
        }
        else if (!isnull(loginIp) && strcmp(loginIp, ip, true)) {
            @return ShowSecretVerifyDialog(playerid);
        }

        mysql_format(dbHandle, query, sizeof(query), "UPDATE characters SET ip = '%e', logged_at = NOW() WHERE id = %d", ip, _:Player.Status[playerid][@id]);
        mysql_pquery(dbHandle, query);

        call OnPlayerLogin(playerid);
        log("player %d verify character and login", playerid);
	}
    mysql_format(dbHandle, query, sizeof(query), "SELECT secret_key, active, ip, DATEDIFF(NOW(), logged_at) as day FROM characters WHERE id = %d", _:Player.Status[playerid][@id]);
    MySQL_TQueryInline(dbHandle, using inline HandleSecurityVerify, query);
    return 1;
}
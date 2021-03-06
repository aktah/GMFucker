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
        SendClientMessage(playerid, X11_INDIANRED, "ACCESS DENIED: "WHITE"�س�١�����ͧ�ҡ��͡�����Ѻ���١��ͧ ��觹�����������˵ؼŴ�ҹ������ʹ���");
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
    Dialog_ShowCallback(playerid, using inline HandleCreationSecretWord, DIALOG_STYLE_PASSWORD, "���ʤ�����ʹ���", ""INDIANRED"��ͤ�����ѧ��ҹ������ʹ���:\n\n"WHITE"��Ң��й��к�����׹�ѹ��ǵ����������Ѻ���ͪ��»���ͧ�ѭ�ռ����\n��觹��ж١�ʴ������������������ͧ͢�س�ա������¹�ŧ�\n\n"WHITE"�س�е�ͧ����觹�����("FIREBRICK"��������索Ҵ!!!"WHITE")\n\n"FIREBRICK"��������Һ��ҹ����������ʼ�ҹ�ͧ�س", "�׹�ѹ", "�͡");
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
    Dialog_ShowCallback(playerid, using inline HandleVerifySecretWord, DIALOG_STYLE_PASSWORD, "���ʤ�����ʹ���", ""INDIANRED"��ͤ�����ѧ��ҹ������ʹ���:\n\n"WHITE"�к��ͧ�������ʶҹС������¹�ŧ����������ͺѭ�բͧ�س\n�����������������ա������Դ������ʹ���\n\n"WHITE"�ô��͡ "FIREBRICK"�����Ѻ"WHITE" ���س���������ҧ���ŧ����¹�����������к�", "�׹�ѹ", "�͡");
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
            
            SendClientMessage(playerid, X11_INDIANRED, "NOTE: ���������Ѥâͧ�س��е���Фâͧ�س�����Ѻ����׹�ѹ �س���������ö��������");
            SendClientMessage(playerid, X11_INDIANRED, "VISIT: "WEBSITE_URL"");
            SendClientMessage(playerid, X11_INDIANRED, "����Фâͧ�س�ѧ������Ѻ����׹�ѹ");
            SendClientMessage(playerid, X11_INDIANRED, "���¤�����þ�ҡ����ҹ "SERVER_ALIAS"");

            defer TimedKick(playerid);
            @return 0;
        }
        else if (day >= Y_PLAYER_SECURE_LOGIN_DAY) {

			SendClientMessage(playerid, X11_YELLOW, "����׹�ѹ���ͧ�ͧ�дѺ");
			SendClientMessage(playerid, X11_YELLOW, "�ѭ�չ��������������к���������ҷ��ҹ�������");
			SendClientMessage(playerid, X11_YELLOW, "�������ѭ�բͧ�س��ʹ��� ��ҵ�ͧ������س�׹�ѹ��������Ңͧ�ѭ�բͧ�س");

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
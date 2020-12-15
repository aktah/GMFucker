#if defined _inc_impl
	#undef _inc_impl
#endif

#include <YSI_Coding\y_hooks>

hook OnGameModeInit()
{
	Command_SetDeniedReturn(true);
	defer onAllInitiated();
}

timer onAllInitiated[100]()
{
	call OnGroupInit();
}

public e_COMMAND_ERRORS:OnPlayerCommandReceived(playerid, cmdtext[], e_COMMAND_ERRORS:success) 
{
    if(!IsLoggedIn(playerid))
    {
		SendClientMessage(playerid, X11_INDIANRED, "ACCESS DENIED: "WHITE"คุณต้องเข้าสู่ระบบก่อนที่จะใช้คำสั่ง");
		return COMMAND_DENIED;
    }

    switch(success) {
    	case COMMAND_DENIED: {
			SendClientMessage(playerid, X11_INDIANRED, "ACCESS DENIED: "WHITE"คุณไม่ได้รับอนุญาตให้ใช้คำสั่งนี้");
    		return COMMAND_DENIED;
    	}
    	case COMMAND_UNDEFINED:
    	{
			if(strlen(cmdtext) > 28) // Preventing long bad commands from returning default message;
				SendClientMessage(playerid, X11_LIGHTBLUE, "ขออภัย, คำสั่งนี้ไม่มีอยู่ ใช้ /help ถ้าคุณต้องการความช่วยเหลือ"); 	
			else
				va_SendClientMessage(playerid, X11_LIGHTBLUE, "ขออภัย, คำสั่ง \"%s\" ไม่มีอยู่ ใช้ /help ถ้าคุณต้องการความช่วยเหลือ", cmdtext);

    		return COMMAND_OK;
    	}
    }
    return success;
}

public OnQueryError(errorid, const error[], const callback[], const query[], MySQL:handle)
{
	switch(errorid) {
		case CR_SERVER_GONE_ERROR: {
			err("Lost connection to server");
		}
		case ER_SYNTAX_ERROR: {
			err("Something is wrong in your syntax, query: %s",query);
		}
		default: {
			err("Other mysql error %d %s (callback %s), query %s", errorid, error, callback, query);
		}
	}
	return 1;
}
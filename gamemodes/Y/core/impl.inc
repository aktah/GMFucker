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
		SendClientMessage(playerid, X11_INDIANRED, "ACCESS DENIED: "WHITE"�س��ͧ�������к���͹����������");
		return COMMAND_DENIED;
    }

    switch(success) {
    	case COMMAND_DENIED: {
			SendClientMessage(playerid, X11_INDIANRED, "ACCESS DENIED: "WHITE"�س������Ѻ͹حҵ��������觹��");
    		return COMMAND_DENIED;
    	}
    	case COMMAND_UNDEFINED:
    	{
			if(strlen(cmdtext) > 28) // Preventing long bad commands from returning default message;
				SendClientMessage(playerid, X11_LIGHTBLUE, "������, ����觹����������� �� /help ��Ҥس��ͧ��ä������������"); 	
			else
				va_SendClientMessage(playerid, X11_LIGHTBLUE, "������, ����� \"%s\" ��������� �� /help ��Ҥس��ͧ��ä������������", cmdtext);

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
#if defined _inc_impl
	#undef _inc_impl
#endif

#include <YSI_Coding\y_hooks>

hook OnGroupInit()
{
	GROUP_ADD<AdminGroup[Y_PLAYER_ADMIN_MANAGE_LEVEL - 1]>
	{
		@YCMD:doors;
		@YCMD:createdoor;
	}
	return 1;
}

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
			cache_get_value_name_int(row, "id", id);

		    idx = Iter_Free(IDoor);

		    if (idx == INVALID_ITERATOR_SLOT) {
		    	err("Number of doors (%d) is greater than the limit (%d)", rows, Y_DOOR_LIMIT);
		    	break;
		    }
        
		    Door.Meta[idx][@id] = DBID:id;
            cache_get_value_name(row, "name", Door.Meta[idx][@name]);
            cache_get_value_name_int(row, "type", Door.Meta[idx][@type]);

            cache_get_value_name_float(row, "eX", Door.Meta[idx][@eX]);
            cache_get_value_name_float(row, "eY", Door.Meta[idx][@eY]);
            cache_get_value_name_float(row, "eZ", Door.Meta[idx][@eZ]);
            cache_get_value_name_float(row, "eA", Door.Meta[idx][@eA]);
            cache_get_value_name_int(row, "eInt", Door.Meta[idx][@eInt]);
            cache_get_value_name_int(row, "eWorld", Door.Meta[idx][@eWorld]);
            cache_get_value_name(row, "eLabel", Door.Meta[idx][@eLabel], Y_DOOR_LABEL_TEXT_LIMIT);
            cache_get_value_name(row, "eGtext", Door.Meta[idx][@eGtext], Y_DOOR_GTEXT_LIMIT);
            cache_get_value_name_int(row, "ePickupModel", Door.Meta[idx][@ePickupModel]);

            cache_get_value_name_float(row, "iX", Door.Meta[idx][@iX]);
            cache_get_value_name_float(row, "iY", Door.Meta[idx][@iY]);
            cache_get_value_name_float(row, "iZ", Door.Meta[idx][@iZ]);
            cache_get_value_name_float(row, "iA", Door.Meta[idx][@iA]);
            cache_get_value_name_int(row, "iInt", Door.Meta[idx][@iInt]);
            cache_get_value_name_int(row, "iWorld", Door.Meta[idx][@iWorld]);
            cache_get_value_name(row, "iLabel", Door.Meta[idx][@iLabel], Y_DOOR_LABEL_TEXT_LIMIT);
            cache_get_value_name(row, "iGtext", Door.Meta[idx][@iGtext], Y_DOOR_GTEXT_LIMIT);
            cache_get_value_name_int(row, "iPickupModel", Door.Meta[idx][@iPickupModel]);
            
		    Iter_Add(IDoor, idx);
		}

        foreach(new doorid:IDoor) {
        
            if (Door.Meta[doorid][@ePickupModel] != 0)
                Door.Meta[doorid][@ePickup] = CreateDynamicPickup(Door.Meta[doorid][@ePickupModel], 23, Door.Meta[doorid][@eX], Door.Meta[doorid][@eY], Door.Meta[doorid][@eZ], Door.Meta[doorid][@eWorld], Door.Meta[doorid][@eInt]);

			if (!isnull(Door.Meta[doorid][@eLabel]))
            	Door.Meta[doorid][@eTLabel] = CreateDynamic3DTextLabel(Door.Meta[doorid][@eLabel], -1, Door.Meta[doorid][@eX], Door.Meta[doorid][@eY], Door.Meta[doorid][@eZ], 5.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, Door.Meta[doorid][@eWorld], Door.Meta[doorid][@eInt]);
            
            if(Door.Meta[doorid][@iPickupModel] != 0)
                Door.Meta[doorid][@iPickup] = CreateDynamicPickup(Door.Meta[doorid][@iPickupModel], 23, Door.Meta[doorid][@iX], Door.Meta[doorid][@iY], Door.Meta[doorid][@iZ], Door.Meta[doorid][@iWorld], Door.Meta[doorid][@iInt]);

			if (!isnull(Door.Meta[doorid][@iLabel]))
            	Door.Meta[doorid][@iTLabel] = CreateDynamic3DTextLabel(Door.Meta[doorid][@iLabel], -1, Door.Meta[doorid][@iX], Door.Meta[doorid][@iY], Door.Meta[doorid][@iZ], 5.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, Door.Meta[doorid][@iWorld], Door.Meta[doorid][@iInt]);
           
			switch(Door.Meta[doorid][@type]) {
				case DOOR_TYPE_CP: {
					Door.Meta[doorid][@eCheckpoint] = CreateDynamicCP(Door.Meta[doorid][@eX], Door.Meta[doorid][@eY], Door.Meta[doorid][@eZ], 3.0, Door.Meta[doorid][@eWorld], Door.Meta[doorid][@eInt], -1, 3.0);
					Door.Meta[doorid][@iCheckpoint] = CreateDynamicCP(Door.Meta[doorid][@iX], Door.Meta[doorid][@iY], Door.Meta[doorid][@iZ], 3.0, Door.Meta[doorid][@iWorld], Door.Meta[doorid][@iInt], -1, 3.0);
				}
				case DOOR_TYPE_CP_EXT: {
					Door.Meta[doorid][@eCheckpoint] = CreateDynamicCP(Door.Meta[doorid][@eX], Door.Meta[doorid][@eY], Door.Meta[doorid][@eZ], 3.0, Door.Meta[doorid][@eWorld], Door.Meta[doorid][@eInt], -1, 3.0);
				}
				case DOOR_TYPE_CP_INT: {
					Door.Meta[doorid][@iCheckpoint] = CreateDynamicCP(Door.Meta[doorid][@iX], Door.Meta[doorid][@iY], Door.Meta[doorid][@iZ], 3.0, Door.Meta[doorid][@iWorld], Door.Meta[doorid][@iInt], -1, 3.0);
				}
			}
        }

		printf("Loaded %d doors", Iter_Count(IDoor));
	}
	MySQL_PQueryInline(dbHandle, using inline Handler, "SELECT * FROM " DOOR_TABLE "");
	return 1;
}

hook OnPlayerEnter(playerid) 
{
	new playerWorld = GetPlayerVirtualWorld(playerid);
	foreach(new doorid:IDoor)
	{
	    if (Door.Meta[doorid][@iX] != 0.0 && Door.Meta[doorid][@iY] != 0.0 && 
			IsPlayerInRangeOfPoint(playerid, 2.5, Door.Meta[doorid][@eX], Door.Meta[doorid][@eY], Door.Meta[doorid][@eZ]) && playerWorld == Door.Meta[doorid][@eWorld]) {

			SetPlayerPos(playerid, Door.Meta[doorid][@iX], Door.Meta[doorid][@iY], Door.Meta[doorid][@iZ]);
			SetPlayerFacingAngle(playerid, Door.Meta[doorid][@iA]);
			SetPlayerInterior(playerid, Door.Meta[doorid][@iInt]);
			SetPlayerVirtualWorld(playerid, Door.Meta[doorid][@iWorld]);
			
			if(!isnull(Door.Meta[doorid][@iGtext])) {
				GameTextForPlayer(playerid, Door.Meta[doorid][@iGtext], 5000, 1);
			}

			return Y_HOOKS_BREAK_RETURN_1;
		}
	}
	return 1;
}

hook OnPlayerExit(playerid) 
{
	new playerWorld = GetPlayerVirtualWorld(playerid);
	foreach(new doorid: IDoor)
	{
		if (IsPlayerInRangeOfPoint(playerid, 2.5, Door.Meta[doorid][@iX], Door.Meta[doorid][@iY], Door.Meta[doorid][@iZ]) && playerWorld == Door.Meta[doorid][@iWorld]) {

			SetPlayerPos(playerid, Door.Meta[doorid][@eX], Door.Meta[doorid][@eY], Door.Meta[doorid][@eZ]);
			SetPlayerFacingAngle(playerid, Door.Meta[doorid][@eA]);
			SetPlayerInterior(playerid, Door.Meta[doorid][@eInt]);
			SetPlayerVirtualWorld(playerid, Door.Meta[doorid][@eWorld]);
			
			if(!isnull(Door.Meta[doorid][@eGtext])) {
				GameTextForPlayer(playerid, Door.Meta[doorid][@eGtext], 5000, 1);
			}

			return Y_HOOKS_BREAK_RETURN_1;
		}
	}
	return 1;
}

RenameDoor(playerid, doorid, bool:error = false)
{
	inline Response(pid, dialogid, response, listitem, string:inputtext[])
	{
		#pragma unused pid, dialogid, response, listitem, inputtext
	
		if(!response) {
			InspectDoor(playerid, doorid);
			@return 1;
		}

		if(isnull(inputtext) || strlen(inputtext) >= Y_DOOR_MAX_NAME) {
			RenameDoor(playerid, doorid, true);
			@return 1;
		}
	
		va_SendClientMessage(playerid, X11_GRAY71, "Door#%d ถูกเปลี่ยนชื่อเป็น "LIGHTBLUE"%s", doorid, inputtext);

		strcpy(Door.Meta[doorid][@name], inputtext);

		new query[60 + Y_DOOR_MAX_NAME + 12 + 1];
		mysql_format(dbHandle, query, sizeof query, "UPDATE " DOOR_TABLE " SET name = '%e' WHERE id = %d", inputtext, _:Door.Meta[doorid][@id]);
		mysql_tquery(dbHandle, query);

		InspectDoor(playerid, doorid);
		@return 1;
	}
	
	new errorText[128];
	if (error) {
		format(errorText, sizeof errorText, ""FIREBRICK"ชื่อต้องอยู่ระหว่าง "LIGHTBLUE"%d"FIREBRICK" และ "LIGHTBLUE"%d"FIREBRICK" ตัวอักษร\n\n", 1, Y_DOOR_MAX_NAME);
	}

	new dialog_string[188 + Y_DOOR_MAX_NAME + 1];
	format(dialog_string, sizeof dialog_string, "%s"WHITE"เปลี่ยนชื่อ Door#%d "LIGHTBLUE"%s"WHITE" พิมพ์ชื่อที่ต้องการ", errorText, doorid, Door.Meta[doorid][@name]);
	Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_INPUT, "แก้ไขชื่อ Door", dialog_string, "ยืนยัน", "กลับ");
	return 1;
}

ChangeTypeDoor(playerid, doorid)
{
	inline Response(pid, dialogid, response, listitem, string:inputtext[])
	{
		#pragma unused pid, dialogid, response, listitem, inputtext
	
		if(!response) {
			InspectDoor(playerid, doorid);
			@return 1;
		}

		Door.Meta[doorid][@type] = listitem;
		va_SendClientMessage(playerid, X11_GRAY71, "Door#%d ถูกเปลี่ยนประเภทเป็น "LIGHTBLUE"%d", doorid, listitem);

		switch(Door.Meta[doorid][@type]) {
			case DOOR_TYPE_CP: {
				if(!IsValidDynamicCP(Door.Meta[doorid][@eCheckpoint])) {
                	Door.Meta[doorid][@eCheckpoint] = CreateDynamicCP(Door.Meta[doorid][@eX], Door.Meta[doorid][@eY], Door.Meta[doorid][@eZ], 3.0, Door.Meta[doorid][@eWorld], Door.Meta[doorid][@eInt], -1, 3.0);
				}

				if(!IsValidDynamicCP(Door.Meta[doorid][@iCheckpoint])) {
					Door.Meta[doorid][@iCheckpoint] = CreateDynamicCP(Door.Meta[doorid][@iX], Door.Meta[doorid][@iY], Door.Meta[doorid][@iZ], 3.0, Door.Meta[doorid][@iWorld], Door.Meta[doorid][@iInt], -1, 3.0);
				}
			}
			case DOOR_TYPE_CP_EXT: {
				if(!IsValidDynamicCP(Door.Meta[doorid][@eCheckpoint])) {
                	Door.Meta[doorid][@eCheckpoint] = CreateDynamicCP(Door.Meta[doorid][@eX], Door.Meta[doorid][@eY], Door.Meta[doorid][@eZ], 3.0, Door.Meta[doorid][@eWorld], Door.Meta[doorid][@eInt], -1, 3.0);
				}

				if(IsValidDynamicCP(Door.Meta[doorid][@iCheckpoint])) {
					DestroyDynamicCP(Door.Meta[doorid][@iCheckpoint]);
				}
			}
			case DOOR_TYPE_CP_INT: {
				if(IsValidDynamicCP(Door.Meta[doorid][@eCheckpoint])) {
                	DestroyDynamicCP(Door.Meta[doorid][@eCheckpoint]);
				}

				if(!IsValidDynamicCP(Door.Meta[doorid][@iCheckpoint])) {
					Door.Meta[doorid][@iCheckpoint] = CreateDynamicCP(Door.Meta[doorid][@iX], Door.Meta[doorid][@iY], Door.Meta[doorid][@iZ], 3.0, Door.Meta[doorid][@iWorld], Door.Meta[doorid][@iInt], -1, 3.0);
				}
			}
			default: {
				if(IsValidDynamicCP(Door.Meta[doorid][@eCheckpoint])) {
                	DestroyDynamicCP(Door.Meta[doorid][@eCheckpoint]);
				}

				if(IsValidDynamicCP(Door.Meta[doorid][@iCheckpoint])) {
                	DestroyDynamicCP(Door.Meta[doorid][@iCheckpoint]);
				}
			}
		}
			
		new query[60];
		mysql_format(dbHandle, query, sizeof query, "UPDATE " DOOR_TABLE " SET type = %d WHERE id = %d", listitem, _:Door.Meta[doorid][@id]);
		mysql_tquery(dbHandle, query);

		InspectDoor(playerid, doorid);
		@return 1;
	}
	Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_LIST, "แก้ไขประเภท Door", "None\nCheckpoint\nCheckpoint (Exterior)\nCheckpoint (Interior)", "ยืนยัน", "กลับ");
	return 1;
}

SetPickUpDoor(playerid, doorid, type)
{
	new dialog_title[80];

	inline Response(pid, dialogid, response, listitem, string:inputtext[])
	{
		#pragma unused pid, dialogid, response, listitem, inputtext

		if(!response) {
			ChangePickUpDoor(playerid, doorid);
			@return 1;
		}

		new pickupModel = strval(inputtext);
		va_SendClientMessage(playerid, X11_GRAY71, "Door#%d ถูกเปลี่ยน %s Pickup เป็น "LIGHTBLUE"%d", doorid, type == 0 ? ("Exterior") : ("Interior"), pickupModel);

		if (type == 0) {
			Door.Meta[doorid][@ePickupModel] = pickupModel;

			if(IsValidDynamicPickup(Door.Meta[doorid][@ePickup])) 
				DestroyDynamicPickup(Door.Meta[doorid][@ePickup]);
			
			if (Door.Meta[doorid][@ePickupModel] != 0)
				Door.Meta[doorid][@ePickup] = CreateDynamicPickup(Door.Meta[doorid][@ePickupModel], 23, Door.Meta[doorid][@eX], Door.Meta[doorid][@eY], Door.Meta[doorid][@eZ], Door.Meta[doorid][@eWorld], Door.Meta[doorid][@eInt]);

			mysql_format(dbHandle, dialog_title, sizeof dialog_title, "UPDATE " DOOR_TABLE " SET ePickupModel = %d WHERE id = %d", pickupModel, _:Door.Meta[doorid][@id]);
		}
		else {
			Door.Meta[doorid][@iPickupModel] = pickupModel;

			if(IsValidDynamicPickup(Door.Meta[doorid][@iPickup])) 
				DestroyDynamicPickup(Door.Meta[doorid][@iPickup]);

	     	if(Door.Meta[doorid][@iPickupModel] != 0)
                Door.Meta[doorid][@iPickup] = CreateDynamicPickup(Door.Meta[doorid][@iPickupModel], 23, Door.Meta[doorid][@iX], Door.Meta[doorid][@iY], Door.Meta[doorid][@iZ], Door.Meta[doorid][@iWorld], Door.Meta[doorid][@iInt]);

			mysql_format(dbHandle, dialog_title, sizeof dialog_title, "UPDATE " DOOR_TABLE " SET iPickupModel = %d WHERE id = %d", pickupModel, _:Door.Meta[doorid][@id]);
		}
		mysql_tquery(dbHandle, dialog_title);

		ChangePickUpDoor(playerid, doorid);
		@return 1;
	}
	format(dialog_title, sizeof dialog_title, "Pickup: %s", type == 0 ? ("Exterior") : ("Interior"));
	Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_INPUT, dialog_title, ""WHITE"ใช้ "FIREBRICK"0"WHITE" เพื่อซ่อนไอคอน\n\nป้อนไอดีไอคอนที่ต้องการในช่องว่างด้านล่างนี้:", "เปลี่ยน", "กลับ");
}

ChangePickUpDoor(playerid, doorid)
{
	inline Response(pid, dialogid, response, listitem, string:inputtext[])
	{
		#pragma unused pid, dialogid, response, listitem, inputtext
	
		if(!response) {
			InspectDoor(playerid, doorid);
			@return 1;
		}
		SetPickUpDoor(playerid, doorid, listitem);
		@return 1;
	}
	new dialog_string[60];
	format(dialog_string, sizeof(dialog_string), "Exterior\t%d\nInterior\t%d", Door.Meta[doorid][@ePickupModel], Door.Meta[doorid][@iPickupModel]);
	Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_TABLIST, "แก้ไขไอคอน Door", dialog_string, "เลือก", "กลับ");
}

SetLabelDoor(playerid, doorid, type, bool:error = false)
{
	new dialog_title[80], dialog_body[160], query[60 + Y_DOOR_LABEL_TEXT_LIMIT + 1];

	inline Response(pid, dialogid, response, listitem, string:inputtext[])
	{
		#pragma unused pid, dialogid, response, listitem, inputtext

		if (!response) {
			ChangeLabelDoor(playerid, doorid);
			@return 1;
		}

		if(strlen(inputtext) >= Y_DOOR_LABEL_TEXT_LIMIT) {
			SetLabelDoor(playerid, doorid, type, true);
			@return 1;
		}

		if (isnull(inputtext)) {
			va_SendClientMessage(playerid, X11_GRAY71, "Door#%d ถูกยกเลิก %s TextLabel", doorid, type == 0 ? ("Exterior") : ("Interior"));
		} else {
			va_SendClientMessage(playerid, X11_GRAY71, "Door#%d ถูกเปลี่ยน %s TextLabel เป็น "LIGHTBLUE"%s", doorid, type == 0 ? ("Exterior") : ("Interior"), inputtext);
		}

		if (type == 0) {
			strcpy(Door.Meta[doorid][@eLabel], inputtext);

			if (isnull(inputtext)) {
				DestroyDynamic3DTextLabel(Door.Meta[doorid][@eTLabel]);
			}
			else if(IsValidDynamic3DTextLabel(Door.Meta[doorid][@eTLabel])) {
				UpdateDynamic3DTextLabelText(Door.Meta[doorid][@eTLabel], -1, Door.Meta[doorid][@eLabel]);
			}
			else {
				Door.Meta[doorid][@eTLabel] = CreateDynamic3DTextLabel(Door.Meta[doorid][@eLabel], -1, Door.Meta[doorid][@eX], Door.Meta[doorid][@eY], Door.Meta[doorid][@eZ], 5.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, Door.Meta[doorid][@eWorld], Door.Meta[doorid][@eInt]);
			}	
			
			mysql_format(dbHandle, query, sizeof query, "UPDATE " DOOR_TABLE " SET eLabel = '%e' WHERE id = %d", inputtext, _:Door.Meta[doorid][@id]);
		}
		else {
			strcpy(Door.Meta[doorid][@iLabel], inputtext);

			if (isnull(inputtext)) {
				DestroyDynamic3DTextLabel(Door.Meta[doorid][@iTLabel]);
			}
			else if(IsValidDynamic3DTextLabel(Door.Meta[doorid][@iTLabel])) {
				UpdateDynamic3DTextLabelText(Door.Meta[doorid][@iTLabel], -1, Door.Meta[doorid][@iLabel]);
			}
			else {
				Door.Meta[doorid][@iTLabel] = CreateDynamic3DTextLabel(Door.Meta[doorid][@iLabel], -1, Door.Meta[doorid][@iX], Door.Meta[doorid][@iY], Door.Meta[doorid][@iZ], 5.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, Door.Meta[doorid][@iWorld], Door.Meta[doorid][@iInt]);
			}

			mysql_format(dbHandle, query, sizeof query, "UPDATE " DOOR_TABLE " SET iLabel = '%e' WHERE id = %d", inputtext, _:Door.Meta[doorid][@id]);
		}
		mysql_tquery(dbHandle, query);

		ChangeLabelDoor(playerid, doorid);
		@return 1;
	}

	new errorText[80];
	if (error) {
		format(errorText, sizeof errorText, ""FIREBRICK"ข้อความต้องไม่มากกว่า "LIGHTBLUE"%d"FIREBRICK" ตัวอักษร\n\n", Y_DOOR_LABEL_TEXT_LIMIT);
	}
	format(dialog_title, sizeof dialog_title, "Textlabel: %s", type == 0 ? ("Exterior") : ("Interior"));
	format(dialog_body, sizeof dialog_body, "%s"WHITE"ปล่อยว่างเพื่อยกเลิกข้อความ\n\nป้อนข้อความที่ต้องการด้านล่างนี้:", errorText);
	Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_INPUT, dialog_title, dialog_body, "เปลี่ยน", "กลับ");
}

ChangeLabelDoor(playerid, doorid)
{
	inline Response(pid, dialogid, response, listitem, string:inputtext[])
	{
		#pragma unused pid, dialogid, response, listitem, inputtext
	
		if(!response) {
			InspectDoor(playerid, doorid);
			@return 1;
		}
		SetLabelDoor(playerid, doorid, listitem);
		@return 1;
	}
	new dialog_string[30 + Y_DOOR_LABEL_TEXT_LIMIT + Y_DOOR_LABEL_TEXT_LIMIT + 2];
	format(dialog_string, sizeof(dialog_string), "Exterior\t%s\nInterior\t%s", Door.Meta[doorid][@eLabel], Door.Meta[doorid][@iLabel]);
	Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_TABLIST, "แก้ไขไอคอน Door", dialog_string, "เลือก", "กลับ");
	return 1;
}

SetGameTextDoor(playerid, doorid, type, bool:error = false)
{
	new dialog_title[80], dialog_body[160], query[60 + Y_DOOR_GTEXT_LIMIT + 1];

	inline Response(pid, dialogid, response, listitem, string:inputtext[])
	{
		#pragma unused pid, dialogid, response, listitem, inputtext

		if (!response) {
			ChangeGTextDoor(playerid, doorid);
			@return 1;
		}

		if(strlen(inputtext) >= Y_DOOR_GTEXT_LIMIT) {
			SetGameTextDoor(playerid, doorid, type, true);
			@return 1;
		}

		if (isnull(inputtext)) {
			va_SendClientMessage(playerid, X11_GRAY71, "Door#%d ถูกยกเลิก %s GameText", doorid, type == 0 ? ("Exterior") : ("Interior"));
		} else {
			va_SendClientMessage(playerid, X11_GRAY71, "Door#%d ถูกเปลี่ยน %s GameText เป็น "LIGHTBLUE"%s", doorid, type == 0 ? ("Exterior") : ("Interior"), inputtext);
		}

		if (type == 0) {
			strcpy(Door.Meta[doorid][@eGtext], inputtext);
			mysql_format(dbHandle, query, sizeof query, "UPDATE " DOOR_TABLE " SET eGtext = '%e' WHERE id = %d", inputtext, _:Door.Meta[doorid][@id]);
		}
		else {
			strcpy(Door.Meta[doorid][@iGtext], inputtext);
			mysql_format(dbHandle, query, sizeof query, "UPDATE " DOOR_TABLE " SET iGtext = '%e' WHERE id = %d", inputtext, _:Door.Meta[doorid][@id]);
		}
		mysql_tquery(dbHandle, query);

		ChangeGTextDoor(playerid, doorid);
		@return 1;
	}

	new errorText[80];
	if (error) {
		format(errorText, sizeof errorText, ""FIREBRICK"ข้อความต้องไม่มากกว่า "LIGHTBLUE"%d"FIREBRICK" ตัวอักษร\n\n", Y_DOOR_GTEXT_LIMIT);
	}
	format(dialog_title, sizeof dialog_title, "GameText: %s", type == 0 ? ("Exterior") : ("Interior"));
	format(dialog_body, sizeof dialog_body, "%s"WHITE"ปล่อยว่างเพื่อยกเลิกข้อความ\n\nป้อนข้อความที่ต้องการด้านล่างนี้:", errorText);
	Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_INPUT, dialog_title, dialog_body, "เปลี่ยน", "กลับ");
}

ChangeGTextDoor(playerid, doorid)
{
	inline Response(pid, dialogid, response, listitem, string:inputtext[])
	{
		#pragma unused pid, dialogid, response, listitem, inputtext
	
		if(!response) {
			InspectDoor(playerid, doorid);
			@return 1;
		}
		SetGameTextDoor(playerid, doorid, listitem);
		@return 1;
	}
	new dialog_string[30 + Y_DOOR_GTEXT_LIMIT + Y_DOOR_GTEXT_LIMIT + 2];
	format(dialog_string, sizeof(dialog_string), "Exterior\t%s\nInterior\t%s", Door.Meta[doorid][@eGtext], Door.Meta[doorid][@iGtext]);
	Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_TABLIST, "แก้ไขไอคอน Door", dialog_string, "เลือก", "กลับ");
	return 1;
}

ChangeWorldDoor(playerid, doorid)
{
	new doors[2048], count;

	format(doors, sizeof doors, "#\tชื่อ");

	foreach(new door:IDoor) {
		if (door == doorid) 
			continue;

		format(doors, sizeof doors, "%s\n"SKYBLUE"%d\t"LIGHTBLUE"%s", doors, door, Door.Meta[door][@name]);	
		count++;	
	}

	format(doors, sizeof doors, "%s\nยกเลิกการเชื่อมต่อ", doors);	

	new 
		idx = 0, 
		linkid = -1,
		query[64]
	;

	inline LinkDoor(pid, dialogid, response, listitem, string:inputtext[])
	{
		#pragma unused pid, dialogid, response, listitem, inputtext

		if(!response) {
			InspectDoor(playerid, doorid);
			@return 1;
		}

		foreach (new door:IDoor) {
			if (door == doorid) 
				continue;

			if (idx++ == listitem) {
				linkid = door;
			}
		}

		if (linkid == -1) {
			va_SendClientMessage(playerid, X11_GRAY71, "Door#%d ถูกยกเลิกการเชื่อมต่อ", doorid);
			Door.Meta[doorid][@iWorld] = Y_DOOR_WORLD_OFFSET + _:Door.Meta[doorid][@id];
		} else {
			new linkworld = Door.Meta[linkid][@iWorld];
			va_SendClientMessage(playerid, X11_GRAY71, "Door#%d ถูกเชื่อมต่อกับ Door#%d "LIGHTBLUE"โลกจำลองที่ %d", doorid, linkid, linkworld);
			Door.Meta[doorid][@iWorld] = linkworld;
		}

		mysql_format(dbHandle, query, sizeof query, "UPDATE " DOOR_TABLE " SET iWorld = %d WHERE id = %d", Door.Meta[doorid][@iWorld], _:Door.Meta[doorid][@id]);
		mysql_tquery(dbHandle, query);

		InspectDoor(playerid, doorid);
	}

	Dialog_ShowCallback(playerid, using inline LinkDoor, DIALOG_STYLE_TABLIST_HEADERS, "การเชื่อมต่อ Door", doors, "เชื่อมต่อ", "ปิด");
	return 1;
}

SetPositionDoor(playerid, doorid, type)
{
	new dialog_body[128], query[160];

	inline Response(pid, dialogid, response, listitem, string:inputtext[])
	{
		#pragma unused pid, dialogid, response, listitem, inputtext

		if (!response) {
			ChangePositionDoor(playerid, doorid);
			@return 1;
		}

		va_SendClientMessage(playerid, X11_GRAY71, "Door#%d ถูกเปลี่ยน "LIGHTBLUE"%s"GRAY71" มายังตำแหน่งปัจจุบันของคุณ", doorid, type == 0 ? ("Exterior") : ("Interior"));

		if (type == 0) {
			GetPlayerPos(playerid, Door.Meta[doorid][@eX], Door.Meta[doorid][@eY], Door.Meta[doorid][@eZ]);
			GetPlayerFacingAngle(playerid, Door.Meta[doorid][@eA]);
			Door.Meta[doorid][@eInt] = GetPlayerInterior(playerid);
			Door.Meta[doorid][@eWorld] = GetPlayerVirtualWorld(playerid);

			if (IsValidDynamicPickup(Door.Meta[doorid][@ePickup]))
				DestroyDynamicPickup(Door.Meta[doorid][@ePickup]);

			if (IsValidDynamic3DTextLabel(Door.Meta[doorid][@eTLabel]))
				DestroyDynamic3DTextLabel(Door.Meta[doorid][@eTLabel]);

			if (IsValidDynamicCP(Door.Meta[doorid][@eCheckpoint]))
				DestroyDynamicCP(Door.Meta[doorid][@eCheckpoint]);

            if(Door.Meta[doorid][@ePickupModel] != 0)
                Door.Meta[doorid][@ePickup] = CreateDynamicPickup(Door.Meta[doorid][@ePickupModel], 23, Door.Meta[doorid][@eX], Door.Meta[doorid][@eY], Door.Meta[doorid][@eZ], Door.Meta[doorid][@eWorld], Door.Meta[doorid][@eInt]);

            Door.Meta[doorid][@eTLabel] = CreateDynamic3DTextLabel(Door.Meta[doorid][@eLabel], -1, Door.Meta[doorid][@eX], Door.Meta[doorid][@eY], Door.Meta[doorid][@eZ], 5.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, Door.Meta[doorid][@eWorld], Door.Meta[doorid][@eInt]);

			switch(Door.Meta[doorid][@type]) {
				case DOOR_TYPE_CP: {
					Door.Meta[doorid][@eCheckpoint] = CreateDynamicCP(Door.Meta[doorid][@eX], Door.Meta[doorid][@eY], Door.Meta[doorid][@eZ], 3.0, Door.Meta[doorid][@eWorld], Door.Meta[doorid][@eInt], -1, 3.0);
				}
				case DOOR_TYPE_CP_EXT: {
					Door.Meta[doorid][@eCheckpoint] = CreateDynamicCP(Door.Meta[doorid][@eX], Door.Meta[doorid][@eY], Door.Meta[doorid][@eZ], 3.0, Door.Meta[doorid][@eWorld], Door.Meta[doorid][@eInt], -1, 3.0);
				}
			}

			mysql_format(dbHandle, query, sizeof query, "UPDATE " DOOR_TABLE " SET eX = %f, eY = %f, eZ = %f, eA = %f, eInt = %i, eWorld = %d WHERE id = %d", 
			Door.Meta[doorid][@eX], Door.Meta[doorid][@eY], Door.Meta[doorid][@eZ], Door.Meta[doorid][@eA], Door.Meta[doorid][@eInt], Door.Meta[doorid][@eWorld], _:Door.Meta[doorid][@id]);
		}
		else {
			GetPlayerPos(playerid, Door.Meta[doorid][@iX], Door.Meta[doorid][@iY], Door.Meta[doorid][@iZ]);
			GetPlayerFacingAngle(playerid, Door.Meta[doorid][@iA]);
			Door.Meta[doorid][@iInt] = GetPlayerInterior(playerid);
			Door.Meta[doorid][@iWorld] = Y_DOOR_WORLD_OFFSET + _:Door.Meta[doorid][@id];

			if (IsValidDynamicPickup(Door.Meta[doorid][@iPickup]))
				DestroyDynamicPickup(Door.Meta[doorid][@iPickup]);

			if (IsValidDynamic3DTextLabel(Door.Meta[doorid][@iTLabel]))
				DestroyDynamic3DTextLabel(Door.Meta[doorid][@iTLabel]);

			if (IsValidDynamicCP(Door.Meta[doorid][@iCheckpoint]))
				DestroyDynamicCP(Door.Meta[doorid][@iCheckpoint]);

            if(Door.Meta[doorid][@iPickupModel] != 0)
                Door.Meta[doorid][@iPickup] = CreateDynamicPickup(Door.Meta[doorid][@iPickupModel], 23, Door.Meta[doorid][@iX], Door.Meta[doorid][@iY], Door.Meta[doorid][@iZ], Door.Meta[doorid][@iWorld], Door.Meta[doorid][@iInt]);

            Door.Meta[doorid][@iTLabel] = CreateDynamic3DTextLabel(Door.Meta[doorid][@iLabel], -1, Door.Meta[doorid][@iX], Door.Meta[doorid][@iY], Door.Meta[doorid][@iZ], 5.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, Door.Meta[doorid][@iWorld], Door.Meta[doorid][@iInt]);
           
			switch(Door.Meta[doorid][@type]) {
				case DOOR_TYPE_CP: {
					Door.Meta[doorid][@iCheckpoint] = CreateDynamicCP(Door.Meta[doorid][@iX], Door.Meta[doorid][@iY], Door.Meta[doorid][@iZ], 3.0, Door.Meta[doorid][@iWorld], Door.Meta[doorid][@iInt], -1, 3.0);
				}
				case DOOR_TYPE_CP_INT: {
					Door.Meta[doorid][@iCheckpoint] = CreateDynamicCP(Door.Meta[doorid][@iX], Door.Meta[doorid][@iY], Door.Meta[doorid][@iZ], 3.0, Door.Meta[doorid][@iWorld], Door.Meta[doorid][@iInt], -1, 3.0);
				}
			}

			mysql_format(dbHandle, query, sizeof query, "UPDATE " DOOR_TABLE " SET iX = %f, iY = %f, iZ = %f, iA = %f, iInt = %i, iWorld = %d WHERE id = %d", 
			Door.Meta[doorid][@iX], Door.Meta[doorid][@iY], Door.Meta[doorid][@iZ], Door.Meta[doorid][@iA], Door.Meta[doorid][@iInt], Door.Meta[doorid][@iWorld], _:Door.Meta[doorid][@id]);
		}
		mysql_tquery(dbHandle, query);

		ChangePositionDoor(playerid, doorid);
		@return 1;
	}

	format(dialog_body, sizeof dialog_body, ""WHITE"คุณแน่ใจหรือที่จะปรับ "LIGHTBLUE"%s"WHITE" นี้มายังตำแหน่งปัจจุบันของคุณ", type == 0 ? ("Exterior (จุดวาร์ปเข้าอาคาร)") : ("Interior (จุดวาร์ปออกอาคาร)"));
	Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_MSGBOX, "แก้ไขจุดวาร์ป", dialog_body, "เปลี่ยน", "กลับ");
}

ChangePositionDoor(playerid, doorid)
{
	inline Response(pid, dialogid, response, listitem, string:inputtext[])
	{
		#pragma unused pid, dialogid, response, listitem, inputtext
	
		if(!response) {
			InspectDoor(playerid, doorid);
			@return 1;
		}
		SetPositionDoor(playerid, doorid, listitem);
		@return 1;
	}
	Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_TABLIST, "แก้ไขทางเข้า/ออก Door", "Exterior\tทางเข้า (จุดวาร์ปเข้าอาคาร)\nInterior\tทางออก (จุดวาร์ปออกอาคาร)", "เลือก", "กลับ");
	return 1;
}

RemoveDoor(playerid, doorid)
{
	new query[128];

	inline Response(pid, dialogid, response, listitem, string:inputtext[])
	{
		#pragma unused pid, dialogid, response, listitem, inputtext
	
		if(!response) {
			InspectDoor(playerid, doorid);
			@return 1;
		}

		inline HandleDoorRemove()
		{
			if (!cache_affected_rows()) {
				@return err("can't remove door %d", doorid);
			}
			memset(Door.Meta[doorid][@name], 0);
			Iter_Remove(IDoor, doorid);
			@return ListDoors(playerid);
		}
		mysql_format(dbHandle, query, sizeof query, "DELETE FROM " DOOR_TABLE" WHERE id = %d", _:Door.Meta[doorid][@id]);
		MySQL_TQueryInline(dbHandle, using inline HandleDoorRemove, query);
		@return 1;
	}
	Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_MSGBOX, "ลบ Door", ""WHITE"คุณแน่ใจหรือที่จะลบ Door นี้\n\n"FIREBRICK"(การกระทำนี้ไม่สามารถย้อนกลับได้)", "ยืนยัน", "กลับ");
}

InspectDoor(playerid, doorid)
{
	log("Player %d inspecting door %d", playerid, doorid);

	inline Response(pid, dialogid, response, listitem, string:inputtext[])
	{
		#pragma unused pid, dialogid, response, listitem, inputtext
	
		if (!response) {
			ListDoors(playerid);

			@return 1;
		}

		if (!IsAdminLevel(playerid, Y_PLAYER_ADMIN_MANAGE_LEVEL)) {
			@return SendClientMessage(playerid, X11_FIREBRICK, "ACCESS DENIED: "WHITE"คุณไม่ได้รับอนุญาต");
		}

		switch (listitem) {
			case DOOR_INSPECT_NAME: {
				@return RenameDoor(playerid, doorid);
			}
			case DOOR_INSPECT_TYPE: {
				@return ChangeTypeDoor(playerid, doorid);
			}
			case DOOR_INSPECT_PICKUP: {
				@return ChangePickUpDoor(playerid, doorid);
			}
			case DOOR_INSPECT_LABEL: {
				@return ChangeLabelDoor(playerid, doorid);
			}
			case DOOR_INSPECT_GAMETEXT: {
				@return ChangeGTextDoor(playerid, doorid);
			}
			case DOOR_INSPECT_WORLD: {
				@return ChangeWorldDoor(playerid, doorid);
			}
			case DOOR_INSPECT_POSITION: {
				@return ChangePositionDoor(playerid, doorid);
			}
			case DOOR_INSPECT_TELEPORT: {
				SetPlayerPos(playerid, Door.Meta[doorid][@eX], Door.Meta[doorid][@eY], Door.Meta[doorid][@eZ]);
				SetPlayerFacingAngle(playerid, Door.Meta[doorid][@eA]);
				SetPlayerInterior(playerid, Door.Meta[doorid][@eInt]);
				SetPlayerVirtualWorld(playerid, Door.Meta[doorid][@eWorld]);
			}
			case DOOR_INSPECT_REMOVE: {
				@return RemoveDoor(playerid, doorid);
			}
			default: {
				err("Player %d chose invalid option %d", playerid, listitem);
			}
		}

		@return InspectDoor(playerid, doorid);
	}

	new dialog_body[145 + Y_DOOR_MAX_NAME + 1], name[Y_DOOR_MAX_NAME + 1];
	if (Door.Meta[doorid][@iWorld] != Y_DOOR_WORLD_OFFSET + _:Door.Meta[doorid][@id]) {
		foreach(new door:IDoor) {
			if (Door.Meta[door][@iWorld] == Door.Meta[doorid][@iWorld]) {
				strcpy(name, Door.Meta[door][@name]);
				break;
			}
		}
	} else {
		strcpy(name, "ไม่ได้เชื่อมต่อ");
	}
	format(dialog_body, sizeof dialog_body, "ชื่อทางเข้า\nประเภท\nไอคอน (Pickup)\nข้อความ (TextLabel)\nข้อความ (GameText)\nการเชื่อมต่อโลก (%s)\nตั้งทางเข้า/ออก\nวาร์ปไปยังทางเข้านี้\nลบ", name);
	Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_LIST, "เลือกสิ่งที่ต้องการทำ", dialog_body, "เลือก", "กลับ");
	return 1;
}

ListDoors(playerid)
{
	inline Noop(pid, dialogid, response, listitem, string:inputtext[])
	{
		#pragma unused pid, dialogid, response, listitem, inputtext
	}

	if (Iter_Count(IDoor) == 0) {
		return Dialog_ShowCallback(playerid, using inline Noop, DIALOG_STYLE_MSGBOX, "ไม่พบข้อมูล", "ไม่พบ Door", "โอเค", "ปิด");
	}

	new doors[2048];

	format(doors, sizeof doors, "#\tชื่อ");

	foreach(new door:IDoor) {
		format(doors, sizeof doors, "%s\n"SKYBLUE"%d\t"LIGHTBLUE"%s", doors, door, Door.Meta[door][@name]);				
	}

	inline Inspect(pid, dialogid, response, listitem, string:inputtext[])
	{
		#pragma unused pid, dialogid, response, listitem, inputtext
	
		if(!response) @return 1;
	
		new
			idx = 0,
			door = -1
		;

		foreach (new doorid:IDoor) {
			if (idx++ == listitem) {
				door = doorid;
			}
		}

		if (door == -1) {
			err("Something went wrong while inspecting doors by %d", playerid);
		}

		if (!IsAdminLevel(playerid, Y_PLAYER_ADMIN_MANAGE_LEVEL)) {
			@return SendClientMessage(playerid, X11_INDIANRED, "ACCESS DENIED: "WHITE"คุณไม่ได้รับอนุญาต");
		}

		InspectDoor(playerid, door);
	
		@return 1;
	}

	Dialog_ShowCallback(playerid, using inline Inspect, DIALOG_STYLE_TABLIST_HEADERS, "Door", doors, "ตรวจสอบ", "ปิด");
	return 1;
}

YCMD:doors(playerid, params[], help)
{
	if(help) return SendClientMessage(playerid, -1, "ไม่รองรับ");

	ListDoors(playerid);

	return 1;
}

YCMD:createdoor(playerid, params[], help)
{
	if(help) return SendClientMessage(playerid, -1, "ไม่รองรับ");

	new
		name[Y_DOOR_MAX_NAME + 1],
		id = Iter_Free(IDoor)
	;

	if (sscanf(params, "s[" #Y_DOOR_MAX_NAME "]", name)) return SendClientMessage(playerid, X11_LIGHTBLUE, "การใช้"WHITE" /createdoor <ชื่อ>");
	if (strlen(name) >= Y_DOOR_MAX_NAME) return va_SendClientMessage(playerid, X11_INDIANRED, "ชื่อ Door ต้องมีความยาวไม่เกิน %d ตัวอักษร", Y_DOOR_MAX_NAME);

	if (id == INVALID_ITERATOR_SLOT)
		return va_SendClientMessage(playerid, X11_INDIANRED, "ERROR: "WHITE"ไม่สามารถสร้าง Door ได้มากกว่านี้แล้ว จำกัดไว้ที่ %d", Y_DOOR_LIMIT);

	new 
		Float:pX, 
		Float:pY, 
		Float:pZ,
		Float:pA,
		pInt,
		pWorld
	;

	GetPlayerPos(playerid, pX, pY, pZ);
	GetPlayerFacingAngle(playerid, pA);
	pInt = GetPlayerInterior(playerid);
	pWorld = GetPlayerVirtualWorld(playerid);

	inline DoorCreated()
	{
		if (!cache_affected_rows()) {
			@return err("can't create door to database");
		}
		Iter_Add(IDoor, id);

		Door.Meta[id][@id] = DBID:cache_insert_id();
		Door.Meta[id][@eX] = pX;
		Door.Meta[id][@eY] = pY;
		Door.Meta[id][@eZ] = pZ;
		Door.Meta[id][@eA] = pA;
		Door.Meta[id][@eInt] = pInt;
		Door.Meta[id][@eWorld] = pWorld;
		strcpy(Door.Meta[id][@name], name);

		SendClientMessage(playerid, X11_KHAKI, "ดำเนินการเสร็จสมบูรณ์แล้ว");

		@return log("door %d was created", id);
	}
	new query[160 + Y_DOOR_MAX_NAME + 1];
	mysql_format(dbHandle, query, sizeof query, "INSERT INTO " DOOR_TABLE " (name,eX,eY,eZ,eA,eInt,eWorld)VALUES('%e',%f,%f,%f,%f,%i,%d)", params, pX, pY, pZ, pA, pInt, pWorld);
	MySQL_PQueryInline(dbHandle, using inline DoorCreated, query);
	return 1;
}

stock GetPlayerLocation(playerid, &Float:fX, &Float:fY, &Float:fZ)
{
    new id = -1;

	if ((id = House_Inside(playerid)) != -1)
    {
        fX = House.Meta[id][@eX];
        fY = House.Meta[id][@eY];
        fZ = House.Meta[id][@eZ];
    }
    // ...
    else GetPlayerPos(playerid, fX, fY, fZ);
}
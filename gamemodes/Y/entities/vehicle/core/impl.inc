#if defined _inc_impl
	#undef _inc_impl
#endif

#include <YSI_Coding\y_hooks>

DestroyCoreVehicle(vehicleid) {

	if (Iter_Contains(IVehicle, vehicleid)) {
		Iter_Remove(IVehicle, vehicleid);
		Vehicle.Meta[vehicleid][@modelid] = 0;
	}

	return DestroyVehicle(vehicleid);
}

CreateCoreVehicle(modelid, Float:x, Float:y, Float:z, Float:a = 0.0, color1 = -1, color2 = -1, respawn_delay = -1, addsiren = 0, int = 0, world = 0, const plate[] = ""SERVER_ALIAS"", bool:engineState = false, bool:lightState = false, bool:lockState = true)
{

	if (Iter_Count(IVehicle) == MAX_VEHICLES) {
		err("vehicle reached to maximum (%d)", MAX_VEHICLES);
		return INVALID_VEHICLE_ID;
	}

	new vehicleid = CreateVehicle(modelid, x, y, z, a, color1, color2, respawn_delay, addsiren);
	
	if(vehicleid > 0 && vehicleid != INVALID_VEHICLE_ID)
	{
		Iter_Add(IVehicle, vehicleid);

		Vehicle.Meta[vehicleid][@modelid] = modelid;
		Vehicle.Meta[vehicleid][@x] = x;
		Vehicle.Meta[vehicleid][@y] = y;
		Vehicle.Meta[vehicleid][@z] = z;
		Vehicle.Meta[vehicleid][@a] = a;
		Vehicle.Meta[vehicleid][@siren] = addsiren;
		Vehicle.Meta[vehicleid][@int] = int;
		Vehicle.Meta[vehicleid][@world] = world;
		Vehicle.Meta[vehicleid][@engine] = engineState;
		Vehicle.Meta[vehicleid][@light] = lightState;
		Vehicle.Meta[vehicleid][@lock] = lockState;

		strcpy(Vehicle.Meta[vehicleid][@plate], plate);

		if (int != 0) {
			LinkVehicleToInterior(vehicleid, int);
		}
		
		if (world != 0) {
			SetVehicleVirtualWorld(vehicleid, world);
		}

		new 
			engine, 
			lights, 
			alarm, 
			doors, 
			bonnet, 
			boot, 
			objective
		;

		GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
		SetVehicleParamsEx(vehicleid, engineState, lightState, alarm, lockState, bonnet, boot, objective);

		SetVehicleNumberPlate(vehicleid, plate);
		SetVehicleZAngle(vehicleid, a);
	}

	return vehicleid;
}

stock ReturnVehicleName(vehicleid)
{
	new
		model = GetVehicleModel(vehicleid),
		name[32] = "None";

    if (model < 400 || model > 611)
	    return name;

	format(name, sizeof(name), gVehicleNames[model - 400]);
	return name;
}

stock ReturnVehicleNameByModel(model)
{
	new
	    name[32] = "None";

    if (model < 400 || model > 611)
	    return name;

	format(name, sizeof(name), gVehicleNames[model - 400]);
	return name;
}
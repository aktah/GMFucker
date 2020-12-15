#if defined _inc_impl
	#undef _inc_impl
#endif

stock GetNearestVehicle(playerid)
{
	new
	 	Float:fX,
	 	Float:fY,
	 	Float:fZ,
	 	Float:fSX,
	    Float:fSY,
		Float:fSZ,
		Float:fRadius;

	foreach (new vehicle: StreamedVehicle[playerid])
	{
		GetVehiclePos(vehicle, fX, fY, fZ);
		GetVehicleModelInfo(Vehicle.Meta[vehicle][@modelid], VEHICLE_MODEL_INFO_SIZE, fSX, fSY, fSZ);

	    fRadius = floatsqroot((fSX + fSX) + (fSY + fSY));
	    if (IsPlayerInRangeOfPoint(playerid, fRadius, fX, fY, fZ))
	    {
	    	return vehicle;
	    }
	}
	return INVALID_VEHICLE_ID;
}
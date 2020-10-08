#if defined _inc_header
	#undef _inc_header
#endif

#include <YSI_Players\y_groups>
#include <YSI_Core\y_utils>
#include <YSI_Coding\y_timers>
#include <YSI_Visual\y_commands>
#include <YSI_Data\y_iterate>
#include <YSI_Core\y_master>

enum E_ADMIN_MSG {
	ADMIN_MSG_INFO,
	ADMIN_MSG_WARNING
}

new Group:AdminGroup[Y_PLAYER_ADMIN_MAX_LEVEL];
new Iterator:IAdminVehicle<MAX_VEHICLES>;
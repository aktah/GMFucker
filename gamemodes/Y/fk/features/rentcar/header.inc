#if defined _inc_header
	#undef _inc_header
#endif

// Dynamic รถเช่า

#define Rentcar. OO_TYPE(RENTCAR,Rentcar)

#define	Y_FK_MAX_RENTCAR	2

#include <YSI_Players\y_groups>
#include <YSI_Visual\y_commands>
#include <YSI_Data\y_iterate>
#include <YSI_Coding\y_inline>
#include <YSI_Extra\y_inline_mysql>
#include <YSI_Server\y_colours>
#include <YSI_Coding\y_va>

enum E_RENTCAR_DATA {
	DBID:E_RENTCAR_Meta_id,
	E_RENTCAR_Meta_car,
	E_RENTCAR_Meta_modelid,
	E_RENTCAR_Meta_price,
	Float:E_RENTCAR_Meta_x,
	Float:E_RENTCAR_Meta_y,
	Float:E_RENTCAR_Meta_z,
	Float:E_RENTCAR_Meta_a,

	E_RENTCAR_Meta_player
}

new RentcarMeta[Y_FK_MAX_RENTCAR][E_RENTCAR_DATA];
new Iterator:IRentcar<Y_FK_MAX_RENTCAR>;

// Rentcar.Meta

#define	Y_FK_TABLE_NAME	"y_fk_rentcar"
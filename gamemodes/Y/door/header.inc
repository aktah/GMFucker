#if defined _inc_header
	#undef _inc_header
#endif

#include <YSI_Core\y_utils>
#include <YSI_Coding\y_inline>
#include <YSI_Data\y_iterate>
#include <YSI_Visual\y_dialog>
#include <YSI_Visual\y_commands>

#define Door. OO_TYPE(DOOR,Door)

enum
{
    DOOR_INSPECT_NAME,
    DOOR_INSPECT_TYPE,
    DOOR_INSPECT_PICKUP,
    DOOR_INSPECT_LABEL,
    DOOR_INSPECT_GAMETEXT,
    DOOR_INSPECT_WORLD,
    DOOR_INSPECT_POSITION,
    DOOR_INSPECT_TELEPORT,
    DOOR_INSPECT_REMOVE
}

enum
{
	DOOR_TYPE_NONE,
	DOOR_TYPE_CP,
    DOOR_TYPE_CP_EXT,
    DOOR_TYPE_CP_INT
}

#define Y_DOOR_LIMIT				100
#define	Y_DOOR_MAX_NAME				30
#define	Y_DOOR_LABEL_TEXT_LIMIT		30
#define	Y_DOOR_GTEXT_LIMIT			30
#define Y_DOOR_DEFAULT_PICKUP 		1314
#define Y_DOOR_WORLD_OFFSET  		2000
#define DOOR_TABLE  "doors"

enum E_DOOR
{
	DBID:E_DOOR_Meta_id,
	E_DOOR_Meta_name[Y_DOOR_MAX_NAME + 1],
	E_DOOR_Meta_type,

    Float:E_DOOR_Meta_eX,
    Float:E_DOOR_Meta_eY,
    Float:E_DOOR_Meta_eZ,
    Float:E_DOOR_Meta_eA,
    E_DOOR_Meta_eInt,
    E_DOOR_Meta_eWorld,
    E_DOOR_Meta_eLabel[Y_DOOR_LABEL_TEXT_LIMIT + 1],
	E_DOOR_Meta_eGtext[Y_DOOR_GTEXT_LIMIT + 1],
	E_DOOR_Meta_ePickupModel,

    Float:E_DOOR_Meta_iX,
    Float:E_DOOR_Meta_iY,
    Float:E_DOOR_Meta_iZ,
    Float:E_DOOR_Meta_iA,
    E_DOOR_Meta_iInt,
    E_DOOR_Meta_iWorld,
    E_DOOR_Meta_iLabel[Y_DOOR_LABEL_TEXT_LIMIT + 1],
	E_DOOR_Meta_iGtext[Y_DOOR_GTEXT_LIMIT + 1],
    E_DOOR_Meta_iPickupModel,

	// game entities

	E_DOOR_Meta_ePickup,
    STREAMER_TAG_3D_TEXT_LABEL:E_DOOR_Meta_eTLabel,
	E_DOOR_Meta_eCheckpoint,

	E_DOOR_Meta_iPickup,
    STREAMER_TAG_3D_TEXT_LABEL:E_DOOR_Meta_iTLabel,
	E_DOOR_Meta_iCheckpoint
};

new DoorMeta[Y_DOOR_LIMIT][E_DOOR];
new Iterator:IDoor<Y_DOOR_LIMIT>;
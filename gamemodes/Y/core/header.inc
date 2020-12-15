#if defined _inc_header
	#undef _inc_header
#endif

#include <YSI_Coding\y_hooks>
#include <YSI_Players\y_groups>
#include <YSI_Server\y_colours>
#include <YSI_Visual\y_commands>
#include <YSI_Coding\y_va>
#include <YSI_Core\y_master>

DEFINE_HOOK_REPLACEMENT(Damage, Dmg);

#define OO_TYPE(%6,%7)%0[%1] (_:@vb:@va:%6:%7@%0[%1]
#define @va:%6:%7@%0[%1][@%2] %7%0[%1])[E_%6_%0_%2]
#define @vb:@va:%6:%7@%0[%1][%2][@%3] %7%0[%1])[%2][E_%6_%0_%3]

#define ALS_DO_GroupInit<%0> %0<GroupInit,>(none:)
#if defined _inc_header
	#undef _inc_header
#endif

#include <a_mysql>

#define INVALID_DB_ID DBID:0

static stock DBID:__DBID__;

new MySQL:dbHandle;

#define ALS_DO_DatabaseInit<%0> %0<DatabaseInit,>(none:)

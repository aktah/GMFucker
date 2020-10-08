#if defined _inc_header
	#undef _inc_header
#endif

#include <log-plugin>

#if defined Y_DEBUG
	#define log printf
	#define err printf
#else
	#define log( Log(systemlog, INFO, 
	#define err( Log(systemlog, ERROR, 
#endif

new Logger:systemlog;
#if defined _inc_Y
	#undef _inc_Y
#endif

#if !defined Y_@@@@_HEADER
	#include "core\header"
	#include "db\header"
	#include "log\header"
	#include "debug\header"
	#include "utils\header"

	#include "door\header"
	#include "entities\header"

	#if defined Y_FUCKER_GM
		#include "fk\header"
	#endif

	#define Y_@@@@_HEADER
#elseif !defined Y_@@@@_END
	#include "core\impl"
	#include "db\impl"
	#include "log\impl"
	#include "debug\impl"
	#include "utils\impl"
	
	#include "door\impl"
	#include "entities\impl"

	#if defined Y_FUCKER_GM
		#include "fk\impl"
	#endif

	#define Y_@@@@_END
#else
	#error Y core already set up
#endif
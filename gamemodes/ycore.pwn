#include <a_samp>

#undef MAX_PLAYERS
#define MAX_PLAYERS (100)

#define FIXES_ServerVarMsg 0 
#include <fixes>

#include <streamer>
#include <sscanf2>
#include <md-sort>

#include "config"

#define YSI_NO_MODE_CACHE
#define YSI_NO_VERSION_CHECK
#define YSI_NO_HEAP_MALLOC

#if defined RUN_TESTS
	#include <YSI_Core\y_testing>
#endif

#define cec_auto
#include <cec>

#include "Y\Y"

#include "Y\Y"

main () {
    print("\n----------------------------------");
	printf(" %.20s aktah", "");
    print(" Author: https://github.com/aktah/");
    print(" ");
    print(" Credits: ");
    print(" Gamemode came from Misiur (https://github.com/Y-samp/Y-Core) and it never finished, it came up for education and the features I need. ");
    print(" This is a project that uses the YSI Library as the main. This project wouldn't have happened without them Y Less, Misiur, Zeex, BlueG, maddinat0r, Incognito and many others.");
    print("----------------------------------\n");
}

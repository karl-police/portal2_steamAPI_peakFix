// Header file
#include "plugin_template.h"

// Console message
typedef void(__cdecl* p_ConMsg)(char const*, ...);
p_ConMsg ConMsg;

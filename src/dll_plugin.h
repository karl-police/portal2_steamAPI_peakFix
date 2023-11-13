// Header file
#include "plugin_template.h"

// Console message
typedef void(__cdecl* p_ConMsg)(char const*, ...);
p_ConMsg ConMsg;


// CreateInterface for Engine
/*typedef void*(__cdecl* p_Engine_CreateInterface)(const char* pName, int* pReturnCode);
p_Engine_CreateInterface Engine_CreateInterface;*/
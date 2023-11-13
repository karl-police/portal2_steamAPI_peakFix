#include <Windows.h>

#include "detours.h"

#include "dll_plugin.h"
#include "steam_api_callbacks.h"

bool g_isPluginLoaded = false;


HMODULE MODULE_tier0;
HMODULE MODULE_steam_api;


typedef void (*pFunc_SteamAPI_RegisterCallback)(class CCallbackBase* pCallback, SteamAPICall_t hAPICall);;
pFunc_SteamAPI_RegisterCallback SteamAPI_RegisterCallback = nullptr;


void hook_SteamAPI_RegisterCallback(class CCallbackBase* pCallback, const int callback) {
	//ConMsg("SteamAPI_RegisterCallback called: \n");
	//ConMsg("Handler: %p, Callback int: %d \n", pCallback, callback);

	if (callback == 304) { // PersonaStateChange
		ConMsg("[Plugin] Callback Register Blocked: Callback: %d\n", callback);
	}
	else {
		// call the original function
		SteamAPI_RegisterCallback(pCallback, callback);
	}
}


//---------------------------------------------------------------------------------
// Purpose: called when the plugin is loaded, load the interface we need from the engine
//---------------------------------------------------------------------------------
bool CEmptyServerPlugin::Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory) {
	// To prevent the plugin from loading when loaded
	if (!g_isPluginLoaded) {
		g_isPluginLoaded = true; // setting it back to false, is not needed, Unload just unloads
	}
	else {
		ConMsg("The plugin is already loaded.\n");
		return false;
	}


	MODULE_tier0 = GetModuleHandleA("tier0.dll");
	MODULE_steam_api = GetModuleHandleA("steam_api.dll");


	// ConMsg
	// get the original function
	ConMsg = (p_ConMsg)GetProcAddress(MODULE_tier0, (LPCSTR)"?ConMsg@@YAXPBDZZ");

	ConMsg("[Plugin] SteamAPI Peak Fix Loaded.\n");


	SteamAPI_RegisterCallback = (pFunc_SteamAPI_RegisterCallback)GetProcAddress(
		MODULE_steam_api, "SteamAPI_RegisterCallback"
	);



	// Detour function to override and keep the original function
	// Deattaching before re-overriding is important to prevent crashes, e.g. when re-injecting
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)SteamAPI_RegisterCallback, hook_SteamAPI_RegisterCallback);
	DetourTransactionCommit();
	
	return true;
}


// There's a convar to manully unload and load plugins
void CEmptyServerPlugin::Unload(void) {
	// Detach hook
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)SteamAPI_RegisterCallback, hook_SteamAPI_RegisterCallback);
	DetourTransactionCommit();
}



// Plugin Description
const char* CEmptyServerPlugin::GetPluginDescription(void)
{
	return "A plugin that fixes stutter peaks caused by SteamAPI calls.";
};


// 
// The plugin is a static singleton that is exported as an interface
//
CEmptyServerPlugin g_EmptyServerPlugin;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CEmptyServerPlugin, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, g_EmptyServerPlugin);
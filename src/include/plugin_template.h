/*
	Plugin tempalte for incomplete stuff.
	You don't need all the files of the SDK to build this.
*/
#pragma once

#include <vector>

typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);
typedef void* (*InstantiateInterfaceFn)();

enum
{
	IFACE_OK = 0,
	IFACE_FAILED
};

// Used internally to register classes.
class InterfaceReg
{
public:
	InterfaceReg(InstantiateInterfaceFn fn, const char* pName);

public:
	InstantiateInterfaceFn	m_CreateFn;
	const char* m_pName;

	InterfaceReg* m_pNext; // For the global list.
	static InterfaceReg* s_pInterfaceRegs;
};



#define DLL_EXPORT				extern "C" __declspec( dllexport )
#define DLL_IMPORT				extern "C" __declspec( dllimport )

#define DLL_CLASS_EXPORT		__declspec( dllexport )
#define DLL_CLASS_IMPORT		__declspec( dllimport )

#define DLL_GLOBAL_EXPORT		extern __declspec( dllexport )
#define DLL_GLOBAL_IMPORT		extern __declspec( dllimport )



// Use this if you want to write the factory function.
#if !defined(_STATIC_LINKED) || !defined(_SUBSYSTEM)
#define EXPOSE_INTERFACE_FN(functionName, interfaceName, versionName) \
	static InterfaceReg __g_Create##interfaceName##_reg(functionName, versionName);
#else
#define EXPOSE_INTERFACE_FN(functionName, interfaceName, versionName) \
	namespace _SUBSYSTEM \
	{	\
		static InterfaceReg __g_Create##interfaceName##_reg(functionName, versionName); \
	}
#endif

#if !defined(_STATIC_LINKED) || !defined(_SUBSYSTEM)
#define EXPOSE_INTERFACE(className, interfaceName, versionName) \
	static void* __Create##className##_interface() {return static_cast<interfaceName *>( new className );} \
	static InterfaceReg __g_Create##className##_reg(__Create##className##_interface, versionName );
#else
#define EXPOSE_INTERFACE(className, interfaceName, versionName) \
	namespace _SUBSYSTEM \
	{	\
		static void* __Create##className##_interface() {return static_cast<interfaceName *>( new className );} \
		static InterfaceReg __g_Create##className##_reg(__Create##className##_interface, versionName ); \
	}
#endif

// Use this to expose a singleton interface with a global variable you've created.
#if !defined(_STATIC_LINKED) || !defined(_SUBSYSTEM)
#define EXPOSE_SINGLE_INTERFACE_GLOBALVAR_WITH_NAMESPACE(className, interfaceNamespace, interfaceName, versionName, globalVarName) \
	static void* __Create##className##interfaceName##_interface() {return static_cast<interfaceNamespace interfaceName *>( &globalVarName );} \
	static InterfaceReg __g_Create##className##interfaceName##_reg(__Create##className##interfaceName##_interface, versionName);
#else
#define EXPOSE_SINGLE_INTERFACE_GLOBALVAR_WITH_NAMESPACE(className, interfaceNamespace, interfaceName, versionName, globalVarName) \
	namespace _SUBSYSTEM \
	{ \
		static void* __Create##className##interfaceName##_interface() {return static_cast<interfaceNamespace interfaceName *>( &globalVarName );} \
		static InterfaceReg __g_Create##className##interfaceName##_reg(__Create##className##interfaceName##_interface, versionName); \
	}
#endif

#define EXPOSE_SINGLE_INTERFACE_GLOBALVAR(className, interfaceName, versionName, globalVarName) \
	EXPOSE_SINGLE_INTERFACE_GLOBALVAR_WITH_NAMESPACE(className, , interfaceName, versionName, globalVarName)

// Use this to expose a singleton interface. This creates the global variable for you automatically.
#if !defined(_STATIC_LINKED) || !defined(_SUBSYSTEM)
#define EXPOSE_SINGLE_INTERFACE(className, interfaceName, versionName) \
	static className __g_##className##_singleton; \
	EXPOSE_SINGLE_INTERFACE_GLOBALVAR(className, interfaceName, versionName, __g_##className##_singleton)
#else
#define EXPOSE_SINGLE_INTERFACE(className, interfaceName, versionName) \
	namespace _SUBSYSTEM \
	{	\
		static className __g_##className##_singleton; \
	}	\
	EXPOSE_SINGLE_INTERFACE_GLOBALVAR(className, interfaceName, versionName, __g_##className##_singleton)
#endif



struct edict_t;
class KeyValues;

class IGameEventListener
{
public:
	virtual	~IGameEventListener(void) {};

	// FireEvent is called by EventManager if event just occured
	// KeyValue memory will be freed by manager if not needed anymore
	virtual void FireGameEvent(KeyValues* event) = 0;
};


class CCommand;

typedef enum
{
	PLUGIN_CONTINUE = 0, // keep going
	PLUGIN_OVERRIDE, // run the game dll function but use our return value instead
	PLUGIN_STOP, // don't run the game dll function at all
} PLUGIN_RESULT;

typedef enum
{
	eQueryCvarValueStatus_ValueIntact = 0,	// It got the value fine.
	eQueryCvarValueStatus_CvarNotFound = 1,
	eQueryCvarValueStatus_NotACvar = 2,		// There's a ConCommand, but it's not a ConVar.
	eQueryCvarValueStatus_CvarProtected = 3	// The cvar was marked with FCVAR_SERVER_CAN_NOT_QUERY, so the server is not allowed to have its value.
} EQueryCvarValueStatus;

typedef int QueryCvarCookie_t;


#define INTERFACEVERSION_ISERVERPLUGINCALLBACKS_VERSION_1	"ISERVERPLUGINCALLBACKS001"
#define INTERFACEVERSION_ISERVERPLUGINCALLBACKS_VERSION_2	"ISERVERPLUGINCALLBACKS002"
#define INTERFACEVERSION_ISERVERPLUGINCALLBACKS				"ISERVERPLUGINCALLBACKS003"

class IServerPluginCallbacks
{
public:
	// Initialize the plugin to run
	// Return false if there is an error during startup.
	virtual bool			Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory) = 0;

	// Called when the plugin should be shutdown
	virtual void			Unload(void) = 0;

	// called when a plugins execution is stopped but the plugin is not unloaded
	virtual void			Pause(void) = 0;

	// called when a plugin should start executing again (sometime after a Pause() call)
	virtual void			UnPause(void) = 0;

	// Returns string describing current plugin.  e.g., Admin-Mod.  
	virtual const char* GetPluginDescription(void) = 0;

	// Called any time a new level is started (after GameInit() also on level transitions within a game)
	virtual void			LevelInit(char const* pMapName) = 0;

	// The server is about to activate
	virtual void			ServerActivate(edict_t* pEdictList, int edictCount, int clientMax) = 0;

	// The server should run physics/think on all edicts
	virtual void			GameFrame(bool simulating) = 0;

	// Called when a level is shutdown (including changing levels)
	virtual void			LevelShutdown(void) = 0;

	// Client is going active
	virtual void			ClientActive(edict_t* pEntity) = 0;

	// Client is fully connected ( has received initial baseline of entities )
	virtual void			ClientFullyConnect(edict_t* pEntity) = 0;

	// Client is disconnecting from server
	virtual void			ClientDisconnect(edict_t* pEntity) = 0;

	// Client is connected and should be put in the game
	virtual void			ClientPutInServer(edict_t* pEntity, char const* playername) = 0;

	// Sets the client index for the client who typed the command into their console
	virtual void			SetCommandClient(int index) = 0;

	// A player changed one/several replicated cvars (name etc)
	virtual void			ClientSettingsChanged(edict_t* pEdict) = 0;

	// Client is connecting to server ( set retVal to false to reject the connection )
	//	You can specify a rejection message by writing it into reject
	virtual PLUGIN_RESULT	ClientConnect(bool* bAllowConnect, edict_t* pEntity, const char* pszName, const char* pszAddress, char* reject, int maxrejectlen) = 0;

	// The client has typed a command at the console
	virtual PLUGIN_RESULT	ClientCommand(edict_t* pEntity, const CCommand& args) = 0;

	// A user has had their network id setup and validated 
	virtual PLUGIN_RESULT	NetworkIDValidated(const char* pszUserName, const char* pszNetworkID) = 0;

	// This is called when a query from IServerPluginHelpers::StartQueryCvarValue is finished.
	// iCookie is the value returned by IServerPluginHelpers::StartQueryCvarValue.
	// Added with version 2 of the interface.
	virtual void			OnQueryCvarValueFinished(QueryCvarCookie_t iCookie, edict_t* pPlayerEntity, EQueryCvarValueStatus eStatus, const char* pCvarName, const char* pCvarValue) = 0;

	// added with version 3 of the interface.
	virtual void			OnEdictAllocated(edict_t* edict) = 0;
	virtual void			OnEdictFreed(const edict_t* edict) = 0;
};

#define INTERFACEVERSION_ISERVERPLUGINHELPERS			"ISERVERPLUGINHELPERS001"


typedef enum
{
	DIALOG_MSG = 0,		// just an on screen message
	DIALOG_MENU,		// an options menu
	DIALOG_TEXT,		// a richtext dialog
	DIALOG_ENTRY,		// an entry box
	DIALOG_ASKCONNECT	// Ask the client to connect to a specified IP address. Only the "time" and "title" keys are used.
} DIALOG_TYPE;




//---------------------------------------------------------------------------------
// Purpose: a sample 3rd party plugin class
//---------------------------------------------------------------------------------
class CEmptyServerPlugin : public IServerPluginCallbacks, public IGameEventListener
{
public:
	CEmptyServerPlugin();
	~CEmptyServerPlugin();

	// IServerPluginCallbacks methods
	virtual bool			Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory);
	virtual void			Unload(void);
	virtual void			Pause(void) {};
	virtual void			UnPause(void) {};
	virtual const char*		GetPluginDescription(void);
	virtual void			LevelInit(char const* pMapName) {};
	virtual void			ServerActivate(edict_t* pEdictList, int edictCount, int clientMax) {};
	virtual void			GameFrame(bool simulating) {};
	virtual void			LevelShutdown(void) {};
	virtual void			ClientActive(edict_t* pEntity) {};
	virtual void			ClientFullyConnect(edict_t* pEntity) {};
	virtual void			ClientDisconnect(edict_t* pEntity) {};
	virtual void			ClientPutInServer(edict_t* pEntity, char const* playername) {};
	virtual void			SetCommandClient(int index) {};
	virtual void			ClientSettingsChanged(edict_t* pEdict) {};
	virtual PLUGIN_RESULT	ClientConnect(bool* bAllowConnect, edict_t* pEntity, const char* pszName, const char* pszAddress, char* reject, int maxrejectlen)
	{
		return PLUGIN_CONTINUE;
	};
	virtual PLUGIN_RESULT	ClientCommand(edict_t* pEntity, const CCommand& args)
	{
		return PLUGIN_CONTINUE;
	};
	virtual PLUGIN_RESULT	NetworkIDValidated(const char* pszUserName, const char* pszNetworkID)
	{
		return PLUGIN_CONTINUE;
	};
	virtual void			OnQueryCvarValueFinished(QueryCvarCookie_t iCookie, edict_t* pPlayerEntity, EQueryCvarValueStatus eStatus, const char* pCvarName, const char* pCvarValue) {};
	virtual void			OnEdictAllocated(edict_t* edict) {};
	virtual void			OnEdictFreed(const edict_t* edict) {};

	// IGameEventListener Interface
	virtual void FireGameEvent(KeyValues* event) {};

	virtual int GetCommandIndex() { return m_iClientCommandIndex; }
private:
	int m_iClientCommandIndex;
};


InterfaceReg* InterfaceReg::s_pInterfaceRegs = nullptr;

InterfaceReg::InterfaceReg(InstantiateInterfaceFn fn, const char* pName) : m_pName(pName)
{
	m_CreateFn = fn;
	m_pNext = s_pInterfaceRegs;
	s_pInterfaceRegs = this;
}

void* CreateInterfaceInternal(const char* pName, int* pReturnCode)
{
	InterfaceReg* pCur;

	for (pCur = InterfaceReg::s_pInterfaceRegs; pCur; pCur = pCur->m_pNext)
	{
		if (strcmp(pCur->m_pName, pName) == 0)
		{
			if (pReturnCode)
				*pReturnCode = IFACE_OK;

			return pCur->m_CreateFn();
		}
	}

	if (pReturnCode)
		*pReturnCode = IFACE_FAILED;

	return NULL;
}

DLL_EXPORT void* CreateInterface(const char* pName, int* pReturnCode)
{
	return CreateInterfaceInternal(pName, pReturnCode);
}



//---------------------------------------------------------------------------------
// Purpose: constructor/destructor
//---------------------------------------------------------------------------------
CEmptyServerPlugin::CEmptyServerPlugin()
{
	m_iClientCommandIndex = 0;
}

CEmptyServerPlugin::~CEmptyServerPlugin()
{
}
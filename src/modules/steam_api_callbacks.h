/*
	Taken from the original steam_api.h file
	Not everything is included, only the things needed. Otherwise the compiler fails.
*/
#pragma once

#include <stdint.h>

typedef uint8_t uint8;
typedef uint64_t uint64;

// steamtypes.h
typedef uint64 SteamAPICall_t;


class CCallbackBase
{
public:
	CCallbackBase() { m_nCallbackFlags = 0; m_iCallback = 0; }
	// don't add a virtual destructor because we export this binary interface across dll's
	virtual void Run(void* pvParam) = 0;
	virtual void Run(void* pvParam, bool bIOFailure, SteamAPICall_t hSteamAPICall) = 0;
	int GetICallback() { return m_iCallback; }
	virtual int GetCallbackSizeBytes() = 0;

protected:
	enum { k_ECallbackFlagsRegistered = 0x01, k_ECallbackFlagsGameServer = 0x02 };
	uint8 m_nCallbackFlags;
	int m_iCallback;
	friend class CCallbackMgr;
};
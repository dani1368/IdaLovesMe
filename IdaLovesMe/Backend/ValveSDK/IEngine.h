#pragma once
#include "../GlobalIncludes.h"
#include "../ValveSDK/vfunc.h"
#include "CBaseEntity.h"

class CBaseEntity;

class IEngine
{
public:
	bool GetPlayerInfo(int index, PlayerInfo* pinfo)
	{
		typedef bool(__thiscall* Fn)(void*, int, PlayerInfo*);
		return vfunc<Fn>(this, 8)(this, index, pinfo);
	}
	bool IsInGame()
	{
		typedef bool(__thiscall* Fn)(void*);
		return vfunc<Fn>(this, 26)(this);
	}
	bool IsConnected()
	{
		typedef bool(__thiscall* Fn)(void*);
		return vfunc<Fn>(this, 27)(this);
	}
};
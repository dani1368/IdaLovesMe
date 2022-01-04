#pragma once
#include "../GlobalIncludes.h"
#include "../ValveSDK/vfunc.h"
#include "CBaseEntity.h"
#include "../ValveSDK/VMatrix.h"

class CBaseEntity;

class IEngine
{
public:
	void GetScreenSize(int& width, int& height) {
		typedef void(__thiscall* Fn)(void*, int&, int&);
		return vfunc<Fn>(this, 5)(this, width, height);
	}
	bool GetPlayerInfo(int index, PlayerInfo* pinfo)
	{
		typedef bool(__thiscall* Fn)(void*, int, PlayerInfo*);
		return vfunc<Fn>(this, 8)(this, index, pinfo);
	}
	int GetLocalPlayerIndex()
	{
		typedef int(__thiscall* Fn)(void*);
		return vfunc<Fn>(this, 12)(this);
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
	const VMatrix& WorldToScreenMatrix()
	{
		typedef VMatrix&(__thiscall* Fn)(void*);
		return vfunc<Fn>(this, 37)(this);
	}
};
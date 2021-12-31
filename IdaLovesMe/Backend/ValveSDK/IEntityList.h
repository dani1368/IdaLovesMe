#pragma once
#include "vfunc.h"
#include "../Globalincludes.h"

#pragma warning (disable : 4715)

class CBaseEntity;

class IEntityList
{
public:

	CBaseEntity* GetClientEntity(int index)
	{
		typedef CBaseEntity* (__thiscall* Fn)(void*, int);

		if (this != NULL)
			return vfunc<Fn>(this, 3)(this, index);
	}

	int NumberOfEntities(bool bIncludeNonNetworkable)
	{
		typedef int(__thiscall* Fn)(void*, bool);

		if (this)
			return vfunc<Fn>(this, 7)(this, bIncludeNonNetworkable);
	}

	int GetHighestEntityIndex()
	{
		typedef int(__thiscall* Fn)(void*);

		if (this)
			return vfunc<Fn>(this, 8)(this);

		return 0;
	}

	CBaseEntity* GetClientEntityFromHandle(ULONG hEnt)
	{
		typedef CBaseEntity* (__thiscall* Fn)(void*, ULONG);

		if (this)
			return vfunc<Fn>(this, 4)(this, hEnt);
	}
};

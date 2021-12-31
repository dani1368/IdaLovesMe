#pragma once
#include "../Globalincludes.h"
#include "vfunc.h"
#include "RecvData.h"

class CBaseEntity {
public:
	bool IsValid() {
		return this != nullptr;
	}

	int GetHealth()
	{
		return this->IsValid() ? *reinterpret_cast<int*>(uintptr_t(this) + 0x100) : 0;
	}

	int GetIndex()
	{
		return this->IsValid() ? *reinterpret_cast<int*>(uintptr_t(this) + 0x64) : 0;
	}

	ClientClass* GetClientClass()
	{
		void* Networkable = (void*)(this + 0x8);
		typedef ClientClass* (__thiscall* OriginalFn)(void*);

		return this->IsValid() ? vfunc<OriginalFn>(Networkable, 2)(Networkable) : 0;
	}
};

struct PlayerInfo
{
	__int64         unknown;            //0x0000 
	union {
		__int64       steamID64;        //0x0008 - SteamID64
		struct {
			int xuidlow;
			int xuidhigh;
		};
	};
	char            szName[128];        //0x0010 - Player Name
	int             userId;             //0x0090 - Unique Server Identifier
	char            szSteamID[20];      //0x0094 - STEAM_X:Y:Z
	char            pad_0x00A8[0x10];   //0x00A8
	unsigned long   iSteamID;           //0x00B8 - SteamID 
	char            szFriendsName[128];
	bool            fakeplayer;			// Is BOT
	bool            ishltv;
	unsigned int    customFiles[4];
	unsigned char   filesDownloaded;
};
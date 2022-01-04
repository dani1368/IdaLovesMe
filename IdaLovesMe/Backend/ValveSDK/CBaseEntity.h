#pragma once
#include "../Globalincludes.h"
#include "vfunc.h"
#include "RecvData.h"
#include "Vector.h"
#include "NetVar.h"
#include <array>

class CBaseEntity {
public:
	bool IsValid() {
		return this != nullptr;
	}

	template <typename T>
	T getProp(const char* prop) {
		auto offs = Cheat::NetVarManager->GetOffs(this->GetClientClass()->m_pNetworkName, prop);
		int* v1 = (int*)(uintptr_t(this) + offs);
		if (v1)
			return (T)v1;

		bool* v2 = (bool*)(uintptr_t(this) + offs);
		if (v2)
			return (T)v2;

		float* v3 = (float*)(uintptr_t(this) + offs);
		if (v3)
			return (T)v3;

		Vector* v4 = (Vector*)(uintptr_t(this) + offs);
		if (v4)
			return (T)v4;

		return nullptr;
	}

	int GetHealth()
	{
		return this->IsValid() ? *reinterpret_cast<int*>(uintptr_t(this) + Cheat::NetVarManager->GetOffs(this->GetClientClass()->m_pNetworkName, "m_iHealth")) : 0;
	}

	int GetIndex()
	{
		return this->IsValid() ? *reinterpret_cast<int*>(uintptr_t(this) + 0x64) : 0;
	}

	int GetTeam()
	{
		return this->IsValid() ? *reinterpret_cast<int*>(uintptr_t(this) + 0xF4) : -1;
	}

	bool IsDormant()
	{
		return this->IsValid() ? *reinterpret_cast<bool*>(uintptr_t(this) + 0xED) : true;
	}

	Vector GetVecMins() {
		return this->IsValid() ? *reinterpret_cast<Vector*>(uintptr_t(this) + Cheat::NetVarManager->GetOffs(this->GetClientClass()->m_pNetworkName, "m_vecMins")) : Vector(0, 0, 0);
	}

	Vector GetVecMaxs() {
		return this->IsValid() ? *reinterpret_cast<Vector*>(uintptr_t(this) + Cheat::NetVarManager->GetOffs(this->GetClientClass()->m_pNetworkName, "m_vecMaxs")) : Vector(0, 0, 0);
	}

	/*Vector GetAbsOrigin() {
		return this->IsValid() ? *reinterpret_cast<Vector*>(uintptr_t(this) + Cheat::NetVarManager->GetOffs(this->GetClientClass()->m_pNetworkName, "m_vecAbsOrigin")) : Vector(0, 0, 0);
	}*/

	Vector GetAbsOrigin()
	{
		if (!this)
			return Vector(0, 0, 0);

		typedef Vector& (__thiscall* OriginalFn)(void*);
		return ((OriginalFn)vfunc<OriginalFn>(this, 10))(this);
	}

	ClientClass* GetClientClass()
	{
		void* Networkable = (void*)(this + 0x8);
		typedef ClientClass* (__thiscall* OriginalFn)(void*);

		return this->IsValid() ? vfunc<OriginalFn>(Networkable, 2)(Networkable) : 0;
	}

	std::array<float, 24>& GetPoseParameter()
	{
		static int _m_flPoseParameter = Cheat::NetVarManager->GetOffs("CBaseAnimating", "m_flPoseParameter");
		return *(std::array<float, 24>*)((uintptr_t)this + _m_flPoseParameter);
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
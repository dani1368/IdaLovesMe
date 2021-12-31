#pragma once

#include <unordered_map>
#include "RecvData.h"

namespace Cheat
{
	class CNetVarManager
	{
	public:
		void Init();
		int GetOffs(const char* tableName, const char* propName);
		bool HookProp(const char* tableName, const char* propName, RecvVarProxyFn func);
		bool HookProp(const char* tableName, const char* propName, RecvVarProxyFn func, RecvVarProxyFn& orig);

	private:
		std::unordered_map<std::string, RecvTable*> tables;

		int getProp(const char* tableName, const char* propName, RecvProp** prop = 0);
		int getProp(RecvTable* recvTable, const char* propName, RecvProp** prop = 0);
		RecvTable* getTable(const char* tableName);
	};

	extern CNetVarManager* NetVarManager;

	/*struct offsets
	{
		DWORD m_ihealth;
		DWORD m_iarmor;
		DWORD m_simtime;
		DWORD m_poseparam;
		DWORD m_observermode;
		DWORD m_isscoped;
		DWORD m_vecvelocity;
		DWORD m_ragdollpos;
		DWORD m_account;
		DWORD m_lifeState;
		DWORD m_hitboxset;
		DWORD m_lowerbodyyawtarget;
		DWORD m_vecorigin;
		DWORD m_tickbase;
		DWORD m_angeyeangles;
		DWORD m_vecviewoff;
		DWORD m_flnextattack;
		DWORD m_aimpunchangle;
		DWORD m_gungameimmunity;
		DWORD m_hactiveweapon;
		DWORD m_bhashelmet;
		DWORD m_flnextprimaryattack;
		DWORD m_iclip1;
		DWORD m_iitemdefinitionindex;
		DWORD m_fLastShotTime;
		DWORD m_flpostponefirereadytime;
		DWORD m_howner;
		DWORD m_iping;
		DWORD m_bclientsideanim;
		DWORD m_sequence;
		DWORD m_bBombTicking;
		DWORD m_nBombSite;
		DWORD m_fduckamount;
		DWORD m_fduckspeed;
		DWORD m_flC4Blow;
		DWORD m_flTimerLength;
		DWORD m_flDefuseLength;
		DWORD m_flDefuseCountDown;
		DWORD m_bBombDefused;
		DWORD m_hBombDefuser;
		DWORD m_hObserverTarget;
		DWORD m_viewPunchAngle;
		DWORD m_flashalpha;
		DWORD m_fFlags;
		DWORD m_angRotation;
		DWORD m_iShotsFired;
		DWORD m_izoomLevel;
		DWORD m_bHasHeavyArmor;
		DWORD m_bPinPulled;
		DWORD m_fThrowTime;
		DWORD m_flNextSecondaryAttack;
		DWORD dwPlayerResource;
		DWORD dwInitKeyValues;
		DWORD dwOverridePostProcessing;
		DWORD dwLoadFromBuffer;
		DWORD dwSetAbsAngles;
		DWORD dwSetAbsOrigin;
		DWORD dwGetSequenceActivity;
		DWORD dwSmokeCount;
		DWORD dwLoadSky;
		DWORD dwCrosshair;
		DWORD dwSetClanTag;
		DWORD dwServerRankRevealAll;
		DWORD dwReloading;
	};
	extern offsets offys;*/
}

#define nv(t, p) Cheat::NetVarManager->getOffs(t, p)

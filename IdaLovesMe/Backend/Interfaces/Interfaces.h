#pragma once
#include "../GlobalIncludes.h"
#include "../ValveSDK/Convar.h"
#include "../ValveSDK/CGlobalVars.h"
#include "../ValveSDK/IClient.h"

typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);

namespace Interfaces
{
	class CInterfaceManager
	{
	public:
		CInterfaceManager(std::string szModuleName);

		template<typename T>
		T* GetInterface(std::string szInterfaceVersion);

		template<typename T>
		T* BruteInterface(std::string szInterface);
	private:
		CreateInterfaceFn m_hCreateInterface;
	};

	class CInterfaces
	{
	protected:
		CInterfaceManager* ClientManager,
			* VSTDLibManager;
	public:
		void Initialize();
		void Free();
	};

	extern CInterfaces* InterfaceLoader;

	extern IClient* Client;
	extern ICvar* Var;
	extern CGlobalVars* Globals;
}
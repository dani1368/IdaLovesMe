#pragma once
#include "../GlobalIncludes.h"
#include "../ValveSDK/ICvar.h"
#include "../ValveSDK/CGlobalVars.h"
#include "../ValveSDK/IClient.h"
#include "../ValveSDK/IEngine.h"
#include "../ValveSDK/IEntityList.h"
#include "../ValveSDK/IPanel.h"
#include "../ValveSDK/ISurface.h"

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
						 * VSTDLibManager,
						 * VGUIManager,
						 * VGUIMatManager,
						 * EngineManager;
	public:
		void Initialize();
		void Free();
	};

	extern CInterfaces* InterfaceLoader;

	extern IClient* Client;
	extern ICvar* Var;
	extern CGlobalVars* Globals;
	extern IEngine* Engine;
	extern IEntityList* EntityList;
	extern IPanel* GuiPanel;
	extern ISurface* Surface;
	extern void* RenderView;
}
#include "Interfaces.h"

using namespace Interfaces;

CInterfaces* Interfaces::InterfaceLoader = new CInterfaces();

CInterfaceManager::CInterfaceManager(std::string szModuleName)
{
	m_hCreateInterface = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(szModuleName.c_str()), ("CreateInterface"));
}

template<typename T>
T* CInterfaceManager::GetInterface(std::string szInterfaceVersion)
{
	return reinterpret_cast<T*>(m_hCreateInterface(szInterfaceVersion.c_str(), nullptr));
}

template<typename T>
T* CInterfaceManager::BruteInterface(std::string szInterfaceVersion)
{
	char buffer[64];
	int count = 0;
	sprintf_s(buffer, ("%s%03i"), szInterfaceVersion.c_str(), 0);
	auto hInterface = reinterpret_cast<T*>(m_hCreateInterface(buffer, nullptr));

	while (hInterface == nullptr)
	{
		count++;

		if (count == 1000)
			return nullptr;

		sprintf_s(buffer, ("%s%03i"), szInterfaceVersion.c_str(), count);
		hInterface = reinterpret_cast<T*>(m_hCreateInterface(buffer, nullptr));
	}

	return hInterface;
}

void CInterfaces::Initialize() {
	EngineManager = new CInterfaceManager("engine.dll");
	ClientManager = new CInterfaceManager("client.dll");
	VSTDLibManager = new CInterfaceManager("vstdlib.dll");
	VGUIManager = new CInterfaceManager("vgui2.dll");
	VGUIMatManager = new CInterfaceManager("vguimatsurface.dll");

	Engine = EngineManager->BruteInterface<IEngine>("VEngineClient");
	Client = ClientManager->BruteInterface<IClient>("VClient");
	Var = VSTDLibManager->BruteInterface<ICvar>("VEngineCvar");
	EntityList = ClientManager->BruteInterface<IEntityList>("VClientEntityList");
	GuiPanel = VGUIManager->BruteInterface<IPanel>("VGUI_Panel");
	Surface = VGUIMatManager->BruteInterface<ISurface>("VGUI_Surface");
	RenderView = EngineManager->BruteInterface<IPanel>("VEngineRenderView");

	Globals = **(CGlobalVars***)((*(DWORD**)(Client))[0] + 0x1F);
}

void CInterfaces::Free()
{
	delete EngineManager;
	delete ClientManager;
	delete VSTDLibManager;
	delete VGUIManager;
	delete Interfaces::InterfaceLoader;
}

IEngine* Interfaces::Engine = nullptr;
IClient* Interfaces::Client = nullptr;
ICvar* Interfaces::Var = nullptr;
CGlobalVars* Interfaces::Globals = nullptr;
IEntityList* Interfaces::EntityList = nullptr;
IPanel* Interfaces::GuiPanel = nullptr;
ISurface* Interfaces::Surface = nullptr;
void* Interfaces::RenderView = nullptr;
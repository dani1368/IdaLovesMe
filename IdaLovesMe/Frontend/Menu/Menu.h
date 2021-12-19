#pragma once
#include "../../Backend/Globalincludes.h"
#include "../../Backend/Misc/singleton.h"

class CMenu : public singleton<CMenu> 
{
public:
	void Initialize();
	void Draw();

	bool IsMenuOpened();
	void SetMenuOpened(bool v);
	
	int Menu_key = VK_INSERT;
private:
	bool m_bInitialized;
	bool m_bIsOpened;
	
	int m_nCurrentTab;
	int m_nCurrentLegitTab;
};
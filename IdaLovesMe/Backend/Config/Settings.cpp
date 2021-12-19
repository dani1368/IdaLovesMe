#include "Settings.h"

CSettings* Settings = new CSettings();

void CSettings::LoadDefaults()
{
	this->UnloadCheat = false;
}

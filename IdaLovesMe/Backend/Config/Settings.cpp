#include "Settings.h"

std::unique_ptr<CSettings> Settings = std::make_unique<CSettings>();

void CSettings::LoadDefaults()
{
	this->UnloadCheat = false;
	this->ResetLayout = false;
}

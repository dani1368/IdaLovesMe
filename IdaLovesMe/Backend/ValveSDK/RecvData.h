#pragma once
#include "../Globalincludes.h"

class ClientClass
{
public:
	void* m_pCreateFn;
	void* m_pCreateEventFn;
	char* m_pNetworkName;
	DWORD* m_pRecvTable;
	ClientClass* m_pNext;
	int				m_ClassID;
};
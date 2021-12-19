#pragma once

template<typename T>
T vfunc(void* pTable, int index)
{
	PDWORD* VTablePointer = (PDWORD*)pTable;
	PDWORD VTableFunctionBase = *VTablePointer;
	DWORD dwAddress = VTableFunctionBase[index];
	return (T)(dwAddress);
}

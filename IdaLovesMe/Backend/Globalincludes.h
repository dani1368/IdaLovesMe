#pragma once

#include <Windows.h>
#include <string>
#include <memory>

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);

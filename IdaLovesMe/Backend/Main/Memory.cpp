#include <algorithm>

#ifdef _WIN32
#include <Windows.h>
#endif

#include "Memory.h"

std::size_t calculateVmtLength(const std::uintptr_t* vmt) noexcept
{
    std::size_t length = 0;
#ifdef _WIN32
    MEMORY_BASIC_INFORMATION memoryInfo;
    while (VirtualQuery(LPCVOID(vmt[length]), &memoryInfo, sizeof(memoryInfo)) && memoryInfo.Protect & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY))
        ++length;
#else
    while (vmt[length])
        ++length;
#endif
    return length;
}

void VmtHook::Init(void* base) noexcept
{
    this->base = base;
    auto vmt = *reinterpret_cast<std::uintptr_t**>(base);
    length = calculateVmtLength(vmt);
    oldVmt = std::make_unique<std::uintptr_t[]>(length);
    std::copy(vmt, vmt + length, oldVmt.get());
}

void VmtHook::Restore() const noexcept
{
    auto vmt = *reinterpret_cast<std::uintptr_t**>(base);
#ifdef _WIN32
    if (DWORD oldProtection; VirtualProtect(vmt, length, PAGE_EXECUTE_READWRITE, &oldProtection)) {
        std::copy(oldVmt.get(), oldVmt.get() + length, vmt);
        VirtualProtect(vmt, length, oldProtection, nullptr);
    }
#endif
}

void VmtHook::HookAt(std::size_t index, void* fun) const noexcept
{
    auto address = *reinterpret_cast<std::uintptr_t**>(base) + index;
#ifdef _WIN32
    if (DWORD oldProtection; VirtualProtect(address, sizeof(address), PAGE_EXECUTE_READWRITE, &oldProtection)) {
        *address = std::uintptr_t(fun);
        VirtualProtect(address, sizeof(address), oldProtection, nullptr);
    }
#endif
}
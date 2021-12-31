#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>

class VmtHook {
public:
    void Init(void* base) noexcept;
    void Restore() const noexcept;
    void HookAt(std::size_t index, void* fun) const noexcept;

    template<typename T>
    inline T GetOriginal(uint32_t index)
    {
        return (T)oldVmt[index];
    }

private:
    void* base;
    std::size_t length;
    std::unique_ptr<std::uintptr_t[]> oldVmt;
};


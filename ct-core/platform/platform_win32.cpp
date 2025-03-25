#include "defines.h"

#if CT_PLATFORM_WINDOWS
#include "pch/pch.h"

#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  ifndef NOMINMAX
#    define NOMINMAX
#  endif
#  ifndef UNICODE
#    define UNICODE
#  endif

#include <Windows.h>
#include <timeapi.h>

#include "ct_platform.h"

static u64 get_page_size()
{
    SYSTEM_INFO system{};
    GetSystemInfo(&system);
    return system.dwPageSize;
}

CT_API void* platform_reserve_memory(u64 size_to_reserve)
{
    static const u64 page_size = get_page_size();
    const u64 allocation_size = get_aligned(size_to_reserve, page_size);

    return VirtualAlloc(nullptr, allocation_size, MEM_RESERVE, PAGE_NOACCESS);
}

CT_API void* platform_commit_memory(void* base_ptr, u64 offset, u64 size_to_commit)
{
    static const u64 page_size = get_page_size();

    u64 offset_size = get_aligned(offset, page_size);
    u64 commit_size = get_aligned(size_to_commit, page_size);

    return VirtualAlloc((u8*)base_ptr + offset_size, commit_size, MEM_COMMIT, PAGE_READWRITE);
}

CT_API void platform_free_memory(void* base_ptr, [[maybe_unused]] u64 size_to_free)
{
    static const u64 page_size = get_page_size();

    base_ptr = (void*)get_aligned((uintptr_t)base_ptr, page_size);
    size_to_free = get_aligned(size_to_free, page_size);

    [[maybe_unused]] BOOL res = VirtualFree(base_ptr, size_to_free, MEM_RELEASE);
}

CT_API void report_assertion_failure(const char* expression, const char* message, const char* file, s32 line)
{//todo
    int *ptr = 0;
    [[maybe_unused]] int a = *ptr; //old fashioned way of crashing
}

#endif //CT_PLATFORM_WINDOWS

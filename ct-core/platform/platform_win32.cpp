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
#include "containers/strings.h"

static int
utf8_to_utf16(const char* utf8, int utf8_len, wchar_t* utf16, int utf16_buff_len)
{
    // First pass in a null bufferr to get the required size.
    int num_chars = MultiByteToWideChar(CP_UTF8, 0, utf8, utf8_len, NULL, 0);

    if (utf16 != nullptr && num_chars > 0) {
        // Convert the string for real this time
        num_chars = MultiByteToWideChar(CP_UTF8, 0, utf8,utf8_len, utf16, utf16_buff_len);
        utf16[num_chars] = L'\0';
    }

    return num_chars;
}

static int
utf16_to_utf8(const wchar_t* utf16, int utf16_len, char* utf8, int utf8_buf_len)
{
    int num_chars = WideCharToMultiByte(CP_UTF8, 0, utf16, utf16_len, NULL, 0, NULL, NULL);

    if (utf8 != NULL && num_chars > 0) {
        num_chars = WideCharToMultiByte(CP_UTF8, 0, utf16, utf16_len, utf8, utf8_buf_len, NULL, NULL);
        utf8[num_chars] = 0;
    }

    return num_chars;
}

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

CT_API platform_error
dynamic_library_load(dynamic_library* self, const char* libname)
{
    wchar_t libpath_w[MAX_PATH];
    utf8_to_utf16(libname, ct_strlen(libname), libpath_w, MAX_PATH);

    self->handle = LoadLibraryW(libpath_w);
    if (self->handle != nullptr) {
        return platform_error::none;
    }
    else {
        return platform_error::cannot_find_dynamic_library;
    }
}

CT_API void
dynamic_library_unload(dynamic_library* self)
{
    if (self->handle)
    {
        FreeLibrary((HMODULE)self->handle);
        self->handle = nullptr;
    }
}

CT_API void*
dynamic_library_load_function(dynamic_library* self, const char* function_name)
{
    if (self->handle)
    {
        return GetProcAddress((HMODULE)self->handle, function_name);
    }

    return nullptr;
}

#endif //CT_PLATFORM_WINDOWS

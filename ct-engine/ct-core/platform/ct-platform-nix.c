#include "../debug/ct-assert.h"
#include "../defines.h"

#if CT_PLATFORM_LINUX
#    include "ct-platform.h"
#    include "pch/pch.h"

#    include <dlfcn.h>
#    include <sys/mman.h>
#    include <unistd.h>

CT_API void*
platform_reserve_memory(u64 size_to_reserve)
{
    const int page_size = getpagesize();
    size_to_reserve = get_aligned(size_to_reserve, page_size);

    void* result = mmap(nullptr, size_to_reserve, PROT_NONE, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
    CT_ASSERT_MSG(result != MAP_FAILED, "Failed to reserve virtual memory");

    return result;
}

CT_API void*
platform_commit_memory(void* base_ptr, u64 offset, u64 size_to_commit)
{
    const int page_size = getpagesize();
    size_to_commit = get_aligned(size_to_commit, page_size);
    offset = get_aligned(offset, page_size);

    int res = mprotect((u8*)base_ptr + offset, size_to_commit, PROT_READ | PROT_WRITE);
    CT_ASSERT_MSG(res == 0, "Failed to map virtual address range");

    return (u8*)base_ptr + offset;
}

CT_API void
platform_free_memory(void* base_ptr, [[maybe_unused]] u64 size_to_free)
{
    const int page_size = getpagesize();
    size_to_free = get_aligned(size_to_free, page_size);

    munmap(base_ptr, size_to_free);
}

CT_API void
report_assertion_failure(const char* expression, const char* message, const char* file, s32 line)
{
    fprintf(stderr, "Assertion failure: %s in file %s:%d.\n\t%s", expression, file, line, message);
}

CT_API platform_error
dynamic_library_load(dynamic_library* self, const char* libname)
{
    platform_error result = platform_error_none;

    self->handle = dlopen(libname, RTLD_NOW | RTLD_LOCAL);
    if (!self->handle) {
        printf("Error: %s\n", dlerror());
        result = platform_error_cannot_find_dynamic_library;
    }

    return result;
}

CT_API void
dynamic_library_unload(dynamic_library* self)
{
    if (self->handle) {
        dlclose(self->handle);
        self->handle = nullptr;
    }
}

CT_API void*
dynamic_library_load_function(dynamic_library* self, const char* function_name)
{
    if (self->handle) {
        return dlsym(self->handle, function_name);
    }

    return nullptr;
}

#endif // CT_PLATFORM_LINUX

#include "pch/pch.h"
#include "ct_memory.h"

#include "allocators/dynamic_allocator.h"
#include "debug/ct_assert.h"
#include "platform/ct_platform.h"

struct memory_stats
{
    u64 total_allocated;
    u64 tagged_allocations[memory::max_tags];
};

struct memory_system_state
{
    u64 total_size_requirement;
    memory_system_config config;
    memory_stats stats;
    u64 dynamic_allocator_requirement;
    dynamic_allocator* heap;
    allocator_i heap_allocator{};
    //@assume single threaded for now
};

memory_system_state* g_state{nullptr};


CT_API bool
memory_system_initialize(memory_system_config config)
{
    u64 allocator_requirement = 0;
    dynamic_allocator_initialize(config.total_alloc_size, &allocator_requirement, nullptr, nullptr);

    u64 total_size_requirement = allocator_requirement;
    total_size_requirement += sizeof(memory_system_state);

    void* base_ptr = platform_reserve_memory(total_size_requirement);
    base_ptr = platform_commit_memory(base_ptr, 0, total_size_requirement);

    g_state = (memory_system_state*)base_ptr;
    ct_zero_memory(base_ptr, total_size_requirement);

    g_state->total_size_requirement = total_size_requirement;
    g_state->dynamic_allocator_requirement = allocator_requirement;
    ct_copy_memory(&g_state->config, &config, sizeof(memory_system_config));

    dynamic_allocator_initialize(config.total_alloc_size, &allocator_requirement, (void*)(g_state + 1), &g_state->heap);

    g_state->heap_allocator = dynamic_allocator_get_interface(g_state->heap);

    return true;
}

CT_API void
memory_system_shutdown()
{
    dynamic_allocator_shutdown(g_state->heap);
    platform_free_memory((void*)g_state, g_state->total_size_requirement);
}

CT_API allocator_i
ct_get_heap_allocator()
{
    return g_state->heap_allocator;
}


CT_API void*
ct_allocate(u64 size, memory::tag tag)
{
    return ct_allocate_aligned(size, 1, tag);
}

CT_API void*
ct_allocate_aligned(u64 size, u16 alignment, memory::tag tag)
{
    CT_ASSERT(tag < memory::max_tags);
    void* result = dynamic_allocator_allocate(g_state->heap, get_aligned(size, alignment));
    if (result)
    {
        g_state->stats.total_allocated += 1;
        g_state->stats.tagged_allocations[tag] += 1;
    }
    return result;
}

CT_API void
ct_free(void* ptr, memory::tag tag)
{
    CT_ASSERT(tag < memory::max_tags);
    if (ptr)
    {
        dynamic_allocator_free(g_state->heap, ptr);
        g_state->stats.total_allocated -= 1;
        g_state->stats.tagged_allocations[tag] -= 1;
    }
}

CT_API void*
ct_reallocate(void* block, u64 old_size, u64 new_size, memory::tag tag)
{
    return ct_reallocate_aligned(block, old_size, new_size, 1, tag);
}

CT_API void*
ct_reallocate_aligned(void* block, u64 old_size, u64 new_size, u16 alignment, memory::tag tag)
{
    old_size = get_aligned(old_size, alignment);
    new_size = get_aligned(new_size, alignment);

    void* new_ptr = ct_allocate_aligned(new_size, 1, tag);
    ct_copy_memory(new_ptr, block, CT_MIN(old_size, new_size));

    ct_free(block, tag);

    return new_ptr;
}

CT_API void
ct_set_memory(void* block, u8 value, u64 size)
{
    const u8* endp = (u8*)block + size;
    for (u8* iter = (u8*)block; iter != endp; ++iter) {
        *iter = 0;
    }
}

CT_API void
ct_zero_memory(void* block, u64 size)
{
    ct_set_memory(block, 0, size);
}

CT_API void
ct_copy_memory(void* dest, const void* source, u64 size)
{
    memcpy(dest, source, size);
}

CT_API u32
pack_u8_into_u32(u8 x, u8 y, u8 z, u8 w)
{
    return x << 24 | y << 16 | z << 8 | w;
}

CT_API void
unpack_u8_from_u32(u32 n, u8& x, u8& y, u8& z, u8& w)
{
    x = (n >> 24) & 0xFF;
    y = (n >> 16) & 0xFF;
    z = (n >> 8) & 0xFF;
    w = n & 0xFF;
}

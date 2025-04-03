#pragma once

namespace memory {
    enum tag {
        unknown,
        array,
        linear_allocator,
        darray,
        hashtable,
        ring_queue,
        string,
        registry,
        plugin,
        platform,
        engine,
        render,
        ui,
        audio,
        game,
        texture,
        material_instance,
        vulkan,
        vulkan_ext, // "External" vulkan allocations, for reporting purposes only.
        gpu_local,
        keymap,

        max_tags,
    };
};

struct allocator_i
{
    void* (*allocate)(allocator_i* allocator, u64 size);
    void (*free)(allocator_i* allocator, void* block);
    void (*free_all)(allocator_i* allocator);
    void* internal_state;
};

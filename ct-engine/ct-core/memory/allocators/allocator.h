#pragma once

#include "../../types.h"

typedef enum memory_tag memory_tag;
enum memory_tag {
    memory_tag_unknown,
    memory_tag_array,
    memory_tag_linear_allocator,
    memory_tag_darray,
    memory_tag_hashtable,
    memory_tag_ring_queue,
    memory_tag_string,
    memory_tag_registry,
    memory_tag_plugin,
    memory_tag_platform,
    memory_tag_engine,
    memory_tag_render,
    memory_tag_ui,
    memory_tag_audio,
    memory_tag_game,
    memory_tag_texture,
    memory_tag_material_instance,
    memory_tag_vulkan,
    memory_tag_vulkan_ext, // "External" vulkan allocations, for reporting purposes only.
    memory_tag_gpu_local,
    memory_tag_keymap,
    memory_tag_max,
};

typedef struct allocator_i allocator_i;
struct allocator_i {
    void* (*allocate)(allocator_i* allocator, u64 size);
    void (*free)(allocator_i* allocator, void* block);
    void (*free_all)(allocator_i* allocator);
    void* internal_state;
};

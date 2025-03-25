#pragma once

#include "types.h"

//
// Memory Handling

// Reserves a region of virtual memory, but does not commit to physical memory
// virtual allocations are always forward aligned to the nearest page size
CT_API void* platform_reserve_memory(u64 size_to_reserve);
// Commits a region of virtual memory to physical memory
CT_API void* platform_commit_memory(void* base_ptr, u64 offset, u64 size_to_commit);
CT_API void platform_free_memory(void* base_ptr, [[maybe_unused]] u64 size_to_free);

//
// Dynamic Library

struct dynamic_library
{
    void* handle;
};

//
// Windowing

enum class window_state : u8
{
    minimized,
    maximized,
    windowed,
    boderless_fullscreen,
};

struct ct_window_config
{
    u32 width{1920};
    u32 height{1080};
    u32 display_index{0}; //zero signifies primary window
    window_state default_state{window_state::windowed};
};

struct ct_window
{
    ct_window_config default_config{};
    void* internal_state;
};

bool window_initialize(ct_window_config* config, ct_window* out_window, u64 *required_size);
bool window_release(ct_window_config* config, ct_window* out_window, u64 *required_size);

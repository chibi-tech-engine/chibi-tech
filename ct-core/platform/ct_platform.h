#pragma once

#include "types.h"

//
// Errors

enum class platform_error : u8 {
    none,
    cannot_load_dynamic_library,
    cannot_find_dynamic_library,
    cannot_find_dynamic_library_function,
};

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

CT_API platform_error dynamic_library_load(dynamic_library* self, const char* libname);
CT_API void dynamic_library_unload(dynamic_library* self);
CT_API void* dynamic_library_load_function(dynamic_library* self, const char* function_name);

template<typename func_type> func_type
dynamic_library_load_function(dynamic_library* self, const char* function_name)
{
    void* result = dynamic_library_load_function(self, function_name);
    return reinterpret_cast<func_type>(result);
}

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

struct ct_window_api {
    void (*get_render_size)(struct ct_window* self, u32* width, u32* height);
    bool (*poll_events)(struct ct_window* self);
};

struct ct_window
{
    ct_window_config default_config{};
    ct_window_api*   vtable;
    void*            internal_state;
};

struct ct_window_plugin_api {
    void (*create_window)(ct_window_config* config, ct_window* out_window, u64* required_size);
    void (*release_window)(ct_window* window);
};

struct ct_window_plugin {
    ct_window_plugin_api* vtable;
    void* internal_state;
};

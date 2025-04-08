#pragma once

#include "../types.h"

//
// Errors

typedef enum platform_error platform_error;
enum platform_error {
    platform_error_none,
    platform_error_cannot_load_dynamic_library,
    platform_error_cannot_find_dynamic_library,
    platform_error_cannot_find_dynamic_library_function,
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

typedef struct dynamic_library dynamic_library;
struct dynamic_library {
    void* handle;
};

CT_API platform_error dynamic_library_load(dynamic_library* self, const char* libname);
CT_API void dynamic_library_unload(dynamic_library* self);
CT_API void* dynamic_library_load_function(dynamic_library* self, const char* function_name);

//
// Windowing

typedef enum window_state window_state;
enum window_state {
    window_state_minimized,
    window_state_maximized,
    window_state_windowed,
    window_state_boderless_fullscreen,
};

typedef struct ct_window_config ct_window_config;
struct ct_window_config {
    u32 width;
    u32 height;
    u32 display_index; // zero signifies primary window
    window_state default_state;
};

typedef struct ct_window_api ct_window_api;
struct ct_window_api {
    void (*get_render_size)(struct ct_window* self, u32* width, u32* height);
    bool (*poll_events)(struct ct_window* self);
};

typedef struct ct_window ct_window;
struct ct_window {
    ct_window_config default_config;
    ct_window_api* vtable;
    void* internal_state;
};

typedef struct ct_window_plugin_api ct_window_plugin_api;
struct ct_window_plugin_api {
    void (*create_window)(ct_window_config* config, ct_window* out_window, u64* required_size);
    void (*release_window)(ct_window* window);
};

typedef struct ct_window_plugin ct_window_plugin;
struct ct_window_plugin {
    ct_window_plugin_api* vtable;
    void* internal_state;
};

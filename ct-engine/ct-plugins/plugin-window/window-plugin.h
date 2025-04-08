#pragma once

#include <types.h>

#include <flecs.h>

typedef struct ct_window_api ct_window_api;
typedef struct ct_window ct_window;

struct ct_window_api {
    bool (*poll_events)(ct_window* self);
    void (*get_size)(ct_window* self, u32* width, u32* height);
    void (*get_render_size)(ct_window* self, u32* width, u32* height);
};

struct ct_window {
    ct_window_api* vtable;
    void* internal_state;
};

typedef struct ct_plugin_window_api ct_plugin_window_api;
struct ct_plugin_window_api {
    struct ct_window* (*create_window)(u32 width, u32 height, const char* title);
    void (*free_window)(struct ct_window* self);
};

typedef struct ct_plugin_window ct_plugin_window;
struct ct_plugin_window {
    ct_plugin_window_api vtable;
    ct_window_api window_vtable;
};

extern ECS_COMPONENT_DECLARE(ct_plugin_window);
extern ECS_COMPONENT_DECLARE(ct_window);

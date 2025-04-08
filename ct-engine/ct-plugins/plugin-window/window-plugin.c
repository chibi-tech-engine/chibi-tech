#include "window-plugin.h"

#include <containers/strings.h>
#include <core/engine.h>
#include <debug/ct-assert.h>
#include <memory/ct-memory.h>
#include <plugin-api/plugin-api.h>

#include <GLFW/glfw3.h>

static constexpr u32 MAJOR_VERSION = 0;
static constexpr u32 MINOR_VERSION = 0;
static constexpr u32 PATCH_VERSION = 1;
static constexpr u32 PLUGIN_VERSION = make_version(MAJOR_VERSION, MINOR_VERSION, PATCH_VERSION);

ECS_COMPONENT_DECLARE(ct_plugin_window);
ECS_COMPONENT_DECLARE(ct_window);

static void
window_system_on_init(ecs_iter_t* it)
{
    int res = glfwInit();
    CT_ASSERT(res > 0);

    ct_write_to_console("Initialzied ct.plugin.window\n");
}

static void
window_system_update(ecs_iter_t* it)
{
    ct_write_to_console("Hello from plugin window system on update\n");
}

// flecs modules have a specific naming conventions
static void
PluginWindowImport(ecs_world_t* world)
{
    ECS_MODULE(world, PluginWindow);

    // Define components
    ECS_COMPONENT_DEFINE(world, ct_plugin_window);
    ECS_COMPONENT_DEFINE(world, ct_window);

    // Define systems
    //

    // on_init_system
    ecs_system(world,
               {.entity = ecs_entity(world, {.name = "ct_window_plugin_on_init", .add = ecs_ids(ecs_dependson(EcsOnStart))}),
                .query.terms =
                    {
                        {ecs_id(ct_plugin_window)},
                    },
                .callback = window_system_on_init});

    // on_update system
    ecs_system(world,
               {.entity = ecs_entity(world, {.name = "ct_window_plugin_on_update", .add = ecs_ids(ecs_dependson(EcsPreUpdate))}),
                .query.terms =
                    {
                        {ecs_id(ct_window)},
                    },
                .callback = window_system_update});

    ct_write_to_console("ECS imported plugin window system\n");
}

static bool
ct_window_poll_events(ct_window* self)
{
    return true;
}

static void
ct_window_get_size(ct_window* self, u32* width, u32* height)
{
}

static void
ct_window_get_render_size(ct_window* self, u32* width, u32* height)
{
}

static struct ct_window*
create_window(u32 width, u32 height, const char* title)
{
    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    return nullptr;
}

static void
free_window(struct ct_window* self)
{
}

PLUGIN_ENTRY_POINT()
{
    plugin_api api = {0};
    api.version = PLUGIN_VERSION;
    api.plugin_settings = nullptr;
    api.internal_state = ct_allocate(sizeof(ct_plugin_window), memory_tag_plugin);

    // Register the plugin with the ecs
    ECS_IMPORT(engine->ecs_state, PluginWindow);

    ecs_singleton_set(engine->ecs_state, ct_plugin_window,
                      {
                          .vtable = {.create_window = create_window, .free_window = free_window},
                          .window_vtable = {.poll_events = ct_window_poll_events,
                                            .get_size = ct_window_get_size,
                                            .get_render_size = ct_window_get_render_size},
                      });

    ct_write_to_console("Hello from plugin window system\n");
    return api;
}

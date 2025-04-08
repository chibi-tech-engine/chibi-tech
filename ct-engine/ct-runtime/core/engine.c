#include "engine.h"

#include <stddef.h>

#include <containers/strings.h>
#include <debug/ct-assert.h>
#include <memory/ct-memory.h>
#include <platform/ct-platform.h>
#include <plugin-api/plugin-api.h>

#include <flecs.h>

typedef struct engine_plugin engine_plugin;
struct engine_plugin {
    plugin_api plugin;
    dynamic_library lib;
    plugin_on_load_pfn on_load;
};

static engine_plugin
load_plugin(const char* plugin_name)
{
    engine_plugin result = {0};

    char filename[1024];
    int chars_written = ct_strformat(filename, sizeof(filename), "./lib%s.so", plugin_name);
    filename[chars_written] = 0;

    printf("Library name: %s\n", filename);

    platform_error res = dynamic_library_load(&result.lib, filename);
    CT_ASSERT(res == platform_error_none);

    result.on_load = dynamic_library_load_function(&result.lib, "plugin_on_load");
    CT_ASSERT(result.on_load != nullptr);

    return result;
}

CT_API engine_state*
engine_create()
{
    memory_system_config memory_config = {_512MB};
    if (!memory_system_initialize(memory_config)) {
        return nullptr;
    }

    engine_state* state = ct_allocate(sizeof(engine_state), memory_tag_engine);
    state->settings.is_running = false;
    state->settings.is_suspended = false;
    state->stage = engine_stage_booting;

    // NOTE: Boot sequence ======================================================================================================
    // Perform the application's boot sequence.

    state->ecs_state = ecs_init();

    // load window plugin
    engine_plugin window_plugin = load_plugin("ct.plugin.window");
    window_plugin.plugin = window_plugin.on_load(state);

    // NOTE: End boot application sequence

    // todo: post-boot system init

    state->stage = engine_stage_booting_complete;

    // NOTE: Initialization sequence ============================================================================================
    state->stage = engine_stage_initializing;

    state->stage = engine_stage_initializing_complete;

    return state;
}

CT_API void
engine_shutdown(engine_state* state)
{
    ecs_fini(state->ecs_state);
}

CT_API bool
engine_run(engine_state* state)
{
    state->stage = engine_stage_running;
    state->settings.is_running = true;

    while (state->settings.is_running) {
        // todo: pump window messages

        if (!state->settings.is_suspended) {
            // todo

            ecs_progress(state->ecs_state, 0);
        }
    }

    state->stage = engine_stage_shutting_down;

    // todo:shutdown the engine

    state->stage = engine_stage_unitialized;

    ct_free(state, memory_tag_engine);
    memory_system_shutdown();

    ct_free(state, memory_tag_engine);

    return true;
}

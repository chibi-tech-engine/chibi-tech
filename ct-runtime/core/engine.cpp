#include "engine.h"

#include "application/application_config.h"
#include "application/application.h"
#include "debug/ct_assert.h"
#include "memory/ct_memory.h"
#include "platform/ct_platform.h"

CT_API bool
engine_create(application* game_state)
{
    memory_system_config memory_config{ _512MB };
    if (!memory_system_initialize(memory_config)) {
        return false;
    }

    game_state->engine_state = ct_allocate(sizeof(engine_state), memory::engine);

    engine_state* state = (engine_state*)game_state->engine_state;
    state->is_running = false;
    state->is_suspended = false;
    //todo:

    // NOTE: Boot sequence ======================================================================================================
    // Perform the application's boot sequence.
    game_state->stage = application_stage::booting;
    if (!game_state->boot(game_state))
    {
        return false;
    }

    // NOTE: End boot application sequence

    //todo: post-boot system init

    game_state->stage = application_stage::booting_complete;

    // NOTE: Initialization sequence ============================================================================================
    game_state->stage = application_stage::initializing;
    if (!game_state->initialize(game_state))
    {
        return false;
    }

    game_state->stage = application_stage::initializing_complete;

    return true;
}

CT_API void
engine_shutdown(application* game_state)
{
}

CT_API bool
engine_run(application* game_state)
{
    game_state->stage = application_stage::running;

    engine_state* state = (engine_state*)game_state->engine_state;
    state->is_running = true;

    while (state->is_running)
    {
        //todo: pump window messages

        if (!state->is_suspended)
        {
            if (!game_state->update(game_state))
            {
                state->is_running = false;
                break;
            }

            if (!game_state->prepare_frame(game_state))
            {
                continue;
            }

            if (!game_state->render_frame(game_state))
            {
                state->is_running = false;
                break;
            }
        }
    }

    game_state->stage = application_stage::shutting_down;
    game_state->shutdown(game_state);

    //todo:shutdown the engine

    ct_free(state, memory::engine);
    memory_system_shutdown();

    game_state->engine_state = nullptr;
    game_state->stage = application_stage::unitialized;

    return true;
}

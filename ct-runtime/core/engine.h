#pragma once

#include <defines.h>
#include <types.h>

struct application;

struct engine_state
{
    bool is_running;
    bool is_suspended;
};

CT_API bool engine_create(application* game_state);
CT_API void engine_shutdown(application* game_state);
CT_API bool engine_run(application* game_inst);

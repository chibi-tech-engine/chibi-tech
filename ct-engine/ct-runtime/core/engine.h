#pragma once

#include <defines.h>
#include <types.h>

struct engine;
struct ecs_world_t;

typedef enum engine_stage engine_stage;
enum engine_stage {
    engine_stage_unitialized,
    engine_stage_booting,
    engine_stage_booting_complete,
    engine_stage_initializing,
    engine_stage_initializing_complete,
    engine_stage_running,
    engine_stage_shutting_down,
};

typedef struct engine_settings engine_settings;
struct engine_settings {
    bool is_running;
    bool is_suspended;
};

typedef struct engine_state engine_state;
struct engine_state {
    engine_settings settings;
    engine_stage stage;
    struct ecs_world_t* ecs_state;
};

CT_API engine_state* engine_create();
CT_API void engine_shutdown(struct engine_state* engine);
CT_API bool engine_run(struct engine_state* engine);

#pragma once

#include "../types.h"

struct engine_state;

typedef struct plugin_api plugin_api;
struct plugin_api {
    u32 version;
    const char* plugin_settings /* = nullptr */;
    void* internal_state;
};

typedef plugin_api (*plugin_on_load_pfn)(struct engine_state* engine);
#define PLUGIN_ENTRY_POINT() CT_API plugin_api plugin_on_load(struct engine_state* engine)

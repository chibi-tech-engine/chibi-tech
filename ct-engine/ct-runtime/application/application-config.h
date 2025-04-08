#pragma once

struct ct_window_config;

typedef struct application_config application_config;
struct application_config {
    const char* name;
    // todo: other settings

    bool headless;
    const struct ct_window_config* window_config;
};

#pragma once

struct ct_window_config;

struct application_config
{
    const char* name{nullptr};
    //todo: other settings

    bool headless{false};
    const ct_window_config* window_config{nullptr};
};

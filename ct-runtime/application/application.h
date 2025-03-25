#pragma once

#include "application_config.h"

#include <platform/ct_platform.h>

enum class application_stage : u8 {
    unitialized,
    booting,
    booting_complete,
    initializing,
    initializing_complete,
    running,
    shutting_down,
};

struct application {
    application_stage stage;
    const application_config* config;

    // internal application state. created and managed by the application.
    void* state;
    // internal engine stated. created and managed by the engine.
    void* engine_state;

    // dynamic library for the application, if it has one.
    dynamic_library game_library;

    // called during apps boot sequence. the application should fill out the
    // application config with the app's specific requirements.
    bool (*boot)(application* app_state);

    // called during the app's initialization sequence, but after the engine's initialization.
    bool (*initialize)(application* app_state);

    // called during the app's update sequence, but before the engine's update (?)
    // todo; pass frame data
    bool (*update)(application* app_state);

    // called during the app's prepare-frame sequence, but before any rendering is done
    // todo; pass frame data
    bool (*prepare_frame)(application* app_state);

    // called during the app's rendering sequence, but before any rendering is done
    // This function should not actually issue graphics api calls, but instead build
    // command buffers that will be passed to the renderer upon completion.
    // todo; pass frame data
    bool (*render_frame)(application* app_state);

    // called when a resize message is recieved by the window.
    //todo: pass in the platform's window
    void (*on_window_resize)(application* app_state, ct_window* client_window);

    // called during the shutdown sequence.
    void (*shutdown)(application* app_state);

    //todo: callbacks for hot-reloading game state.
};

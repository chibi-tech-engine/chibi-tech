#pragma once

#include "application/application.h"
#include "core/engine.h"

extern bool create_application(application* out_app);
extern bool initialize_application(application* app);

int
main(void)
{
    application app_state = {0};

    if (!create_application(&app_state)) { // failed to create the application
        return -1;
    }

    if (!app_state.render_frame || !app_state.prepare_frame || !app_state.update || !app_state.initialize) {
        return -2;
    }

    if (!engine_create(&app_state)) {
        return 1;
    }

    if (!initialize_application(&app_state)) {
        return -1;
    }

    // Begin the engine loop.
    if (!engine_run(&app_state)) {
        return 2;
    }

    return 0;
}

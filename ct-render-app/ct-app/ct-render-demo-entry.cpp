
#include <application/application.h>
#include <application/application_config.h>
#include <entry.h>
#include <memory/ct_memory.h>
#include <types.h>

static constexpr ct_window_config c_window_config{
    .width = 1920,
    .height = 1080,
    .display_index = 0,
    .default_state = window_state::windowed,
};

static constexpr application_config c_app_config{
    .name = "Render Demo",
    .headless = false,
    .window_config = &c_window_config,
};

static bool app_boot(application* app_state);
static bool app_initialize(application* app_state);
static bool app_update(application* app_state);
static bool app_prepare_frame(application* app_state);
static bool app_render_frame(application* app_state);
static void app_on_window_resize(application* app_state, ct_window* client_window);
static void app_shutdown(application* app_state);

bool
create_application(application* out_app)
{
    out_app->boot = app_boot;
    out_app->initialize = app_initialize;
    out_app->update = app_update;
    out_app->prepare_frame = app_prepare_frame;
    out_app->render_frame = app_render_frame;
    out_app->on_window_resize = app_on_window_resize;
    out_app->shutdown = app_shutdown;
    return true;
}

bool
initialize_application(application* app)
{
    return true;
}

static bool
app_boot(application* app_state)
{
    app_state->config = &c_app_config;
    return true;
}

static bool
app_initialize(application* app_state)
{
    return true;
}

static bool
app_update(application* app_state)
{
    return true;
}

static bool
app_prepare_frame(application* app_state)
{
    return true;
}

static bool
app_render_frame(application* app_state)
{
    return true;
}

static void
app_on_window_resize(application* app_state, ct_window* client_window)
{
}

static void
app_shutdown(application* app_state)
{
}

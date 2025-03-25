#include "defines.h"

#if CT_WAYLAND_WINDOW

#include "ct_platform.h"

#include <wayland-client.h>

struct wayland_window
{
    struct wl_display* display{nullptr};
};

//https://developer.orbitrc.io/documentation/wayland/guides/hello-wayland/
bool window_initialize(ct_window_config* config, ct_window* out_window, u64 *required_size)
{
    return true;
}

bool window_release(ct_window_config* config, ct_window* out_window, u64 *required_size)
{
    return true;
}

#endif //CT_WAYLAND_WINDOW

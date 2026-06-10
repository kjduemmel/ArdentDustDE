#pragma once

#include <wayland-server-core.h>
#include <wlr/backend.h>
#include <wlr/render/wlr_renderer.h>
#include <wlr/render/allocator.h>
#include <wlr/types/wlr_scene.h>
#include <wlr/types/wlr_xdg_shell.h>

struct server {
    struct wl_display *display;

    struct wlr_backend *backend;
    struct wlr_renderer *renderer;
    struct wlr_allocator *allocator;

    struct wlr_scene *scene;

    struct wlr_xdg_shell *xdg_shell;

    struct wl_listener new_output;
    struct wl_listener new_input;
    struct wl_listener new_surface;
};

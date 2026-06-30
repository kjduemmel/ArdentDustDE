#pragma once

#include <wayland-server-core.h>
#include <wlr/backend.h>
#include <stdbool.h>
#include <wlr/render/wlr_renderer.h>
#include <wlr/render/allocator.h>
#include <wlr/types/wlr_scene.h>
#include <wlr/types/wlr_xdg_shell.h>
#include <wlr/types/wlr_output_layout.h>
#include <wlr/types/wlr_cursor.h>
#include <wlr/types/wlr_xcursor_manager.h>

struct server {
    struct wl_display *display;

    struct wlr_backend *backend;
    struct wlr_renderer *renderer;
    struct wlr_allocator *allocator;

    struct wlr_scene *scene;
    struct wlr_scene_tree *scene_tree;

    struct wlr_output_layout *output_layout;

    struct wlr_xdg_shell *xdg_shell;
    
	// ---- Cursor Subsystems ----
	struct wlr_cursor *cursor;
	struct wlr_xcursor_manager *cursor_mgr;

	// ---- Cursor Motion Event Trackers ----
	struct wl_listener cursor_motion;
	struct wl_listener cursor_motion_absolute;

    struct wl_listener new_output;
    struct wl_listener new_surface;
};

bool server_init(struct server *server);
void server_run(struct server *server);
void server_destroy(struct server *server);

void handle_new_output(struct wl_listener *listener, void *data);
void handle_new_surface(struct wl_listener *listener, void *data);



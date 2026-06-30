#define _POSIX_C_SOURCE 200809L

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

#include "server.h"

// ---- Forward Declarations for Event Handlers ----
void handle_cursor_motion(struct wl_listener *listener, void *data);
void handle_cursor_motion_absolute(struct wl_listener *listener, void *data);
void handle_new_output(struct wl_listener *listener, void *data);
void handle_new_surface(struct wl_listener *listener, void *data);


bool server_init(struct server *server) {
    server->display = wl_display_create();
    if (!server->display) return false;

    server->backend = wlr_backend_autocreate(
        wl_display_get_event_loop(server->display),
        NULL
    );
    if (!server->backend) return false;

    server->renderer = wlr_renderer_autocreate(server->backend);
    server->allocator = wlr_allocator_autocreate(
        server->backend,
        server->renderer
    );
    if (!server->renderer || !server->allocator) return false;

    server->scene = wlr_scene_create();
    server->scene_tree =
        wlr_scene_tree_create(&server->scene->tree);

    server->output_layout =
        wlr_output_layout_create(server->display);

    wlr_scene_attach_output_layout(server->scene, server->output_layout);

    server->xdg_shell =
        wlr_xdg_shell_create(server->display, 3);

    // ---- listeners (wiring only) ----
    server->new_output.notify = handle_new_output;
    wl_signal_add(&server->backend->events.new_output,
                  &server->new_output);

    server->new_surface.notify = handle_new_surface;
    wl_signal_add(&server->xdg_shell->events.new_surface,
                  &server->new_surface);


	// ---- Cursor ----
	server->cursor = wlr_cursor_create();
    wlr_cursor_attach_output_layout(server->cursor, server->output_layout); //setup monitor space
    server->cursor_mgr = wlr_xcursor_manager_create("default", 24); //load pointer icons

    server->cursor_motion.notify = handle_cursor_motion; //notification coming through from device
    wl_signal_add(&server->cursor->events.motion, &server->cursor_motion); //connection to previous notification
    // Note: cursor moves relative to previous position on this one [cursor moves # pixels up and right each frame]

	//same thing but absolute position
    server->cursor_motion_absolute.notify = handle_cursor_motion_absolute;
    wl_signal_add(&server->cursor->events.motion_absolute, &server->cursor_motion_absolute);
    //Note: cursor is moved to exact position on this [cursor moves to 60% vertical and 35% horizontal]


    return true;
}

void server_run(struct server *server) {
    const char *socket = wl_display_add_socket_auto(server->display);
    if (!socket) return;

    setenv("WAYLAND_DISPLAY", socket, 1);

    if (!wlr_backend_start(server->backend)) {
		return;
	}
	
	pid_t pid = fork();
	if (pid == 0) {
		execl("/usr/bin/foot", "foot", NULL);
		execl("/usr/bin/alacritty", "alacritty", NULL);
		exit(1);
	}

    wl_display_run(server->display);
}

void server_destroy(struct server *server) {
	//clean up cursor
	if (server->cursor_mgr) wlr_xcursor_manager_destroy(server->cursor_mgr);
	if (server->cursor) wlr_cursor_destroy(server->cursor);
	
    // Pass the global multi-monitor layout tracker
    wlr_output_layout_destroy(server->output_layout);

    // Pass the base root node of your 2D scene graph tree
    wlr_scene_node_destroy(&server->scene->tree.node);

    // Pass the hardware backend wrapper
    wlr_backend_destroy(server->backend);

    // Terminate any running application windows safely
    wl_display_destroy_clients(server->display);

    // Free the master display loop memory
    wl_display_destroy(server->display);
}



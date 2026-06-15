#define _POSIX_C_SOURCE 200809L

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "server.h"

bool server_init(struct server *server) {
    server->display = wl_display_create();

    server->backend = wlr_backend_autocreate(
        wl_display_get_event_loop(server->display),
        NULL
    );

    server->renderer = wlr_renderer_autocreate(server->backend);
    server->allocator = wlr_allocator_autocreate(
        server->backend,
        server->renderer
    );
    
	if (!server->display) {
		return false;
	}

	if (!server->backend) {
		return false;
	}

	if (!server->renderer || !server->allocator) {
		return false;
	}

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

    return true;
}

void server_run(struct server *server) {
    const char *socket = wl_display_add_socket_auto(server->display);
    if (!socket) return;

    setenv("WAYLAND_DISPLAY", socket, 1);

    if (!wlr_backend_start(server->backend)) {
		return;
	}

    wl_display_run(server->display);
}

void server_destroy(struct server *server) {
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



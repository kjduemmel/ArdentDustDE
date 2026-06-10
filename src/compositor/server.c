#include "server.h"
#include <stdlib.h>
#include <wlr/backend.h>
#include <wlr/render/wlr_renderer.h>
#include <wlr/render/allocator.h>
#include <wlr/types/wlr_scene.h>
#include <wlr/types/wlr_xdg_shell.h>

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

    server->scene = wlr_scene_create();

    server->xdg_shell = wlr_xdg_shell_create(server->display, 3);

    return true;
}

void server_run(struct server *server) {
    const char *socket = wl_display_add_socket_auto(server->display);

    if (!socket) {
        return;
    }

    setenv("WAYLAND_DISPLAY", socket, 1);

    wlr_backend_start(server->backend);

    wl_display_run(server->display);
}

void server_destroy(struct server *server) {
    wl_display_destroy_clients(server->display);
    wl_display_destroy(server->display);
}

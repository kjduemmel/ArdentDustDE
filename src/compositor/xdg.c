#include "server.h"

void handle_new_surface(struct wl_listener *listener, void *data) {
    struct server *server =
        wl_container_of(listener, server, new_surface);

    struct wlr_xdg_surface *xdg_surface = data;

    if (xdg_surface->role != WLR_XDG_SURFACE_ROLE_TOPLEVEL)
        return;

    wlr_scene_xdg_surface_create(
        server->scene_tree,
        xdg_surface
    );
}

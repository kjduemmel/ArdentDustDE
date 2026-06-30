#include <wlr/types/wlr_cursor.h>
#include <wlr/types/wlr_xcursor_manager.h>
#include "server.h"

void server_move_cursor_relative(struct server *server, struct wlr_pointer_motion_event *event) {
    wlr_cursor_move(server->cursor, &event->pointer->base, event->delta_x, event->delta_y);
    wlr_cursor_set_xcursor(server->cursor, server->cursor_mgr, "left_ptr");
}

void server_move_cursor_absolute(struct server *server, struct wlr_pointer_motion_absolute_event *event) {
    wlr_cursor_warp_absolute(server->cursor, &event->pointer->base, event->x, event->y);
    wlr_cursor_set_xcursor(server->cursor, server->cursor_mgr, "left_ptr");
}

#include <wayland-server-core.h>
#include <wlr/types/wlr_cursor.h>
#include <wlr/types/wlr_pointer.h>
#include <wlr/types/wlr_input_device.h>
#include "server.h"

// Forward declarations of your actual logical modules
void server_move_cursor_relative(struct server *server, struct wlr_pointer_motion_event *event);
void server_move_cursor_absolute(struct server *server, struct wlr_pointer_motion_absolute_event *event);

void handle_new_input(struct wl_listener *listener, void *data)
{
	struct server *server = wl_container_of(listener, server, new_input);
	struct wlr_input_device *device = data;
	
	switch (device->type)
	{
		case WLR_INPUT_DEVICE_POINTER:
			wlr_cursor_attach_input_device(server->cursor, device);
			break;
			
		case WLR_INPUT_DEVICE_KEYBOARD:
			//TODO: setup keyboard
			break;
			
		default:
			break;
	}
}

// Central listener for relative pointer motion
void handle_cursor_motion(struct wl_listener *listener, void *data) {
    struct server *server = wl_container_of(listener, server, cursor_motion);
    struct wlr_pointer_motion_event *event = data;

    // Route to cursor file
    server_move_cursor_relative(server, event);
}

// Central listener for absolute pointer motion
void handle_cursor_motion_absolute(struct wl_listener *listener, void *data) {
    struct server *server = wl_container_of(listener, server, cursor_motion_absolute);
    struct wlr_pointer_motion_absolute_event *event = data;

    // Route to cursor file
    server_move_cursor_absolute(server, event);
}

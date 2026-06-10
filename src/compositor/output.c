#define _POSIX_C_SOURCE 200809L

#include "server.h"
#include <time.h>

static void output_frame(struct wl_listener *listener, void *data) {
    struct wlr_output *output = data;

    struct wlr_scene_output *scene_output = output->data;

    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);

    wlr_scene_output_commit(scene_output, NULL);
    wlr_scene_output_send_frame_done(scene_output, &now);
}

void handle_new_output(struct wl_listener *listener, void *data) {
    struct server *server =
        wl_container_of(listener, server, new_output);

    struct wlr_output *output = data;

    wlr_output_init_render(output, server->allocator, server->renderer);

    wlr_output_layout_add_auto(server->output_layout, output);

    struct wlr_scene_output *
        scene_output = wlr_scene_output_create(server->scene, output);

    output->data = scene_output;

    static struct wl_listener frame;
    frame.notify = output_frame;
    wl_signal_add(&output->events.frame, &frame);
}

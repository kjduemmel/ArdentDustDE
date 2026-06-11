#define _POSIX_C_SOURCE 200809L

#include "server.h"
#include <time.h>
#include <stdlib.h>

static void handle_output_destroy(struct wl_listener *listener, void *data);

struct output_state {
    struct wlr_output *output;
    struct wlr_scene_output *scene_output;
    struct wl_listener frame;
    struct wl_listener destroy;
};

static void output_frame(struct wl_listener *listener, void *data) {
    struct output_state *state =
        wl_container_of(listener, state, frame);

    struct wlr_scene_output *scene_output = state->scene_output;

    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);

    wlr_scene_output_commit(scene_output, NULL);
    wlr_scene_output_send_frame_done(scene_output, &now);
}

void handle_new_output(struct wl_listener *listener, void *data) {
    struct server *server =
        wl_container_of(listener, server, new_output);

    struct wlr_output *output = data;
    
	// Configure the output
	struct wlr_output_state state;
	wlr_output_state_init(&state);

	wlr_output_state_set_enabled(&state, true);

	struct wlr_output_mode *mode =
		wlr_output_preferred_mode(output);

	if (mode) {
		wlr_output_state_set_mode(&state, mode);
	}

	wlr_output_commit_state(output, &state);
	wlr_output_state_finish(&state);

	// Initialize rendering 
    wlr_output_init_render(output,
        server->allocator,
        server->renderer);

    wlr_output_layout_add_auto(
        server->output_layout,
        output);
        
    wlr_output_preferred_mode(output);

    struct output_state *state =
        calloc(1, sizeof(*state));

    if (!state) {
        return;
    }

    struct wlr_scene_output *scene_output =
        wlr_scene_output_create(server->scene, output);

    state->output = output;
    state->scene_output = scene_output;

    output->data = state;

    state->frame.notify = output_frame;
    wl_signal_add(&output->events.frame,
                  &state->frame);

    state->destroy.notify = handle_output_destroy;
    wl_signal_add(&output->events.destroy,
                  &state->destroy);
}

static void handle_output_destroy(struct wl_listener *listener, void *data) {
    struct output_state *state =
        wl_container_of(listener, state, destroy);

    wl_list_remove(&state->frame.link);
    wl_list_remove(&state->destroy.link);

    wlr_scene_output_destroy(state->scene_output);

    free(state);
}

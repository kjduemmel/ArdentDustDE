#include "server.h"

int main(void) {
    struct server server;

    if (!server_init(&server)) {
        return 1;
    }

    server_run(&server);
    server_destroy(&server);

    return 0;
}

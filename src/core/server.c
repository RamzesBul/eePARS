#include <core/server.h>

typedef struct {
    const char *uri;
    p_server server;
} server_params_t, *p_server_params;

/**
 * @brief Launch server.
 *
 * @param args Server arguments.
 */
static void *server_run(void *args);

/**
 * @brief Server event handler.
 *
 * @param c Connection.
 * @param ev Event.
 * @param ev_data Event data.
 * @param fn_data Function data.
 */
static void server_event_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data);

p_server register_server(const char *uri) {
    p_server this = malloc(sizeof(server_t));

    pthread_create(&this->server_thread, NULL, server_run, &(server_params_t) {
            .server = this,
            .uri = uri,
    });

    return this;
}

void release_server(p_server server) {
    pthread_join(server->server_thread, &(void*){NULL});
    mg_mgr_free(&server->mgr);
    free(server);
}

static void *server_run(void *args) {
    p_server_params serverParams = (p_server_params)args;

    mg_mgr_init(&serverParams->server->mgr);
    mg_http_listen(&serverParams->server->mgr, serverParams->uri, server_event_handler, NULL);
    for (;;) mg_mgr_poll(&serverParams->server->mgr, 1000);

    pthread_exit(NULL);
}

static void server_event_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        if (mg_http_match_uri(hm, "/api/hello")) {
            mg_http_reply(c, 200, "", "{%m:%d}\n", MG_ESC("status"), 1);
        } else {
            struct mg_http_serve_opts opts = {.root_dir = "."};
            mg_http_serve_dir(c, hm, &opts);
        }
    }
}

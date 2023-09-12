#include <core/server.h>
#include <core/client.h>
#include <cesanta/frozen.h>

/**
 * Server launch params.
 */
typedef struct {
    char *uri;
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

p_server register_server(const char *uri, const char *settings) {
    p_server this = malloc(sizeof(server_t));
    p_server_settings serverSettings = malloc(sizeof(server_settings_t));

    if (this && serverSettings) {
        this->server_settings = serverSettings;

        const char *json = json_fread(settings);
        const int size = strlen(json);

        json_scanf(json, size, "{SERVER_SETTINGS:{CORS_POLICY:%Q}}", &this->server_settings->cors_policy);

        p_server_params serverParams = malloc(sizeof(server_params_t));
        if (serverParams) {
            serverParams->server = this;
            serverParams->uri = uri;

            pthread_create(&this->server_thread, NULL, server_run, serverParams);

            return this;
        }

        return NULL;
    }

    if (this) free(this);
    if (serverSettings) free(serverSettings);

    return NULL;
}

void release_server(p_server server) {
    if (server) {
        pthread_join(server->server_thread, &(void *) {});
        mg_mgr_free(&server->mgr);
        free(server->server_settings->cors_policy);
        free(server->server_settings);
        free(server);
    }
}

static void *server_run(void *args) {
    p_server_params serverParams = (p_server_params) args;

    mg_mgr_init(&serverParams->server->mgr);
    mg_http_listen(&serverParams->server->mgr, serverParams->uri, server_event_handler, serverParams);
    for (;;) mg_mgr_poll(&serverParams->server->mgr, 1000);

    pthread_exit(NULL);
}

static void server_event_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    if (ev == MG_EV_HTTP_MSG) {
        char *response = NULL;

        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        if (mg_http_match_uri(hm, "/api/hello")) {
            p_server_params serverParams = (p_server_params) fn_data;
            mg_http_reply(c, 200, serverParams->server->server_settings->cors_policy,
                          "{%m:{%m:\"%s\",%m:%d}}\n",
                          MG_ESC("User"),
                          MG_ESC("Name"), "Danil",
                          MG_ESC("Age"), 23);
        } else {
            struct mg_http_serve_opts opts = {.root_dir = "."};
            mg_http_serve_dir(c, hm, &opts);
        }

        if (response) free(response);
    }
}

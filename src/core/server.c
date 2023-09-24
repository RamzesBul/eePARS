#include <core/server.h>
#include <core/client.h>
#include <cesanta/frozen.h>

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

p_server register_server(const char *settings) {
    p_server this = malloc(sizeof(server_t));

    p_server_settings serverSettings = malloc(sizeof(server_settings_t));
    if (this && serverSettings) {
        this->server_settings = serverSettings;

        const char *json = json_fread(settings);
        const int size = strlen(json);

        json_scanf(json, size, "{SERVER_SETTINGS:{CORS_POLICY:%Q}}", &this->server_settings->cors_policy);
        json_scanf(json, size, "{SERVER_SETTINGS:{BFF_HOST:%Q}}", &this->server_settings->host);

        pthread_create(&this->server_thread, NULL, server_run, this);

        return this;
    }

    if (serverSettings) free(serverSettings);
    if (this) free(this);

    return NULL;
}

void release_server(p_server this) {
    if (this) {
        pthread_join(this->server_thread, &(void *) {});
        mg_mgr_free(&this->mgr);
        if (this->server_settings->host) free(this->server_settings->host);
        if (this->server_settings->cors_policy) free(this->server_settings->cors_policy);
        if (this->server_settings) free(this->server_settings);

        free(this);
    }
}

static void *server_run(void *args) {
    p_server server = (p_server) args;

    mg_mgr_init(&server->mgr);
    mg_http_listen(&server->mgr, server->server_settings->host, server_event_handler, server);
    for (;;) mg_mgr_poll(&server->mgr, 1000);

    pthread_exit(NULL);
}

static void server_event_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    if (ev == MG_EV_HTTP_MSG) {
        p_server server = (p_server) fn_data;

        char *response = NULL;

        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        if (mg_http_match_uri(hm, "/api/authorize")) {
            char *access_token_start = strstr(hm->query.ptr, "access_token=");
            char *access_token_end = strstr(hm->query.ptr, " HTTP");
            int access_token_length = (int)((long long)access_token_end - (long long)access_token_start);

            if (!access_token_start || !access_token_end || !access_token_length) {
                return;
            }

            char token[256];
            strncpy(token, access_token_start, access_token_length);
            token[access_token_length] = 0;

            char request_buff[512];
            sprintf(request_buff, "https://api.vk.com/method/account.getProfileInfo?%s&v=5.154", token);

            response = request_get(request_buff);
            mg_http_reply(c, 200, server->server_settings->cors_policy, "%s", response);
        } else {
            struct mg_http_serve_opts opts = {.root_dir = "."};
            mg_http_serve_dir(c, hm, &opts);
        }

        if (response) free(response);
    }
}

#include <core/client.h>

p_client register_client(void) {
    p_client this = malloc(sizeof(client_t));

    return this;
}

void release_client(p_client this) {
    free(this);
}

typedef struct {
    const char *url;
    bool done;
    char *response;
} fn_data_t;

/**
 * @brief Callback function for client.
 *
 * @param c Connection @b (reserved).
 * @param ev Event @b (reserved).
 * @param ev_data Event data @b (reserved).
 * @param fn_data Function data.
 */
static void default_request_callback(struct mg_connection *c, int ev,
                                     void *ev_data, void *fn_data) {
    fn_data_t *data = (fn_data_t*) fn_data;

    if (ev == MG_EV_CONNECT) {
        // Connected to server. Extract host name from URL
        struct mg_str host = mg_url_host(data->url);

        // If s_url is https://, tell client connection to use TLS
        if (mg_url_is_ssl(data->url)) {
            struct mg_tls_opts opts = {.ca = "../credentials/_.vk.crt", .srvname = host};
            mg_tls_init(c, &opts);
        }

        // Send request
        mg_printf(c,
                  "GET %s HTTP/1.0\r\n"
                  "Host: %.*s\r\n"
                  "\r\n",
                  mg_url_uri(data->url), (int)host.len, host.ptr);
    } else if (ev == MG_EV_HTTP_MSG) {
        // Response is received. Print it
        struct mg_http_message *hm = (struct mg_http_message *)ev_data;
        data->response = malloc((int)hm->message.len + 1);
        strncpy(data->response, hm->message.ptr, (int)hm->message.len);
        c->is_closing = 1;       // Tell mongoose to close this connection
        data->done = true; // Tell event loop to stop
    } else if (ev == MG_EV_ERROR) {
        data->done = true; // Error, tell event loop to stop
    }
}

char * request_get(const char *url) {
    fn_data_t fn_data = {.url = url, .done = false};

    mg_log_set(MG_LL_NONE); // Set log level.

    struct mg_mgr mgr; // Event manager.
    mg_mgr_init(&mgr); // Initialise event manager.

    struct mg_connection *c; // Client connection.
    c = mg_http_connect(&mgr, url, default_request_callback,
                        &fn_data); // Create client.
    while (c && fn_data.done == false)
        mg_mgr_poll(&mgr, 1000); // Wait for echo.

    mg_mgr_free(&mgr); // Deallocate resources.

    return fn_data.response;
}

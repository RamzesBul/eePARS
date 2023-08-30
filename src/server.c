#include <server.h>

/**
 * @brief Callback function for server.
 *
 * @param c Connection @b (reserved).
 * @param ev Event @b (reserved).
 * @param ev_data Event data @b (reserved).
 * @param fn_data Function data.
 */
static void default_server_callback(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *)ev_data;
        if (mg_http_match_uri(hm, "/api/hello")) { // On /api/hello requests,
            mg_http_reply(c, 200, "", "{%m:%d}\n",
                          MG_ESC("status"), 1);                 // Send dynamic JSON response
        } else {                                                // For all other URIs,
            struct mg_http_serve_opts opts = {.root_dir = "."}; // Serve files
            mg_http_serve_dir(c, hm, &opts);                    // From root_dir
        }
    }
}

void *server_listener(void *args) {
    sla_t *server_listener_args = (sla_t *)args;                                    // Cast arguments to server_listener_args.
    const char *port = server_listener_args->port;                                  // Port to listen on.
    mg_event_handler_t callback_function = server_listener_args->callback_function; // Callback function.

    if (!callback_function)
        callback_function = default_server_callback; // Set default callback function.

    char host[128];                                   // Host string.
    sprintf(host, "http://0.0.0.0:%s", (char *)port); // Add port to host string.

    struct mg_mgr mgr;                                   // Event manager.
    mg_mgr_init(&mgr);                                   // Initialize event manager.
    mg_http_listen(&mgr, host, callback_function, NULL); // Listen for HTTP requests.
    for (;;)
        mg_mgr_poll(&mgr, 1000); // Infinite event loop.
}
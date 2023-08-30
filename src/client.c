#include <client.h>

/**
 * @brief Callback function for client.
 *
 * @param c Connection @b (reserved).
 * @param ev Event @b (reserved).
 * @param ev_data Event data @b (reserved).
 * @param fn_data Function data.
 */
static void default_request_callback(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    if (ev == MG_EV_OPEN) {
        c->is_hexdumping = 1; // Enable hexdumping for connection.
    } else if (ev == MG_EV_ERROR) {
        MG_ERROR(("%p %s", c->fd, (char *)ev_data)); // Log error message.
    } else if (ev == MG_EV_WS_OPEN) {
        mg_ws_send(c, "hello", 5, WEBSOCKET_OP_TEXT); // Send message.
    } else if (ev == MG_EV_WS_MSG) {
        struct mg_ws_message *wm = (struct mg_ws_message *)ev_data;          // Get message.
        printf("GOT ECHO REPLY: [%.*s]\n", (int)wm->data.len, wm->data.ptr); // Print message.
    }

    if (ev == MG_EV_ERROR || ev == MG_EV_CLOSE || ev == MG_EV_WS_MSG) {
        *(bool *)fn_data = true; // Signal that we're done.
    }
}

void request_get(const char *url, mg_event_handler_t callback_function) {
    if (!callback_function)
        callback_function = default_request_callback; // Set default callback function.

    mg_log_set(MG_LL_DEBUG); // Set log level.

    struct mg_mgr mgr; // Event manager.
    mg_mgr_init(&mgr); // Initialise event manager.

    bool done = false; // Event handler flips it to true.

    struct mg_connection *c;                                      // Client connection.
    c = mg_ws_connect(&mgr, url, callback_function, &done, NULL); // Create client.
    while (c && done == false)
        mg_mgr_poll(&mgr, 1000); // Wait for echo.

    mg_mgr_free(&mgr); // Deallocate resources.
}

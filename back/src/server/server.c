#include <server/server.h>

#include <stdlib.h>

#include <cesanta/frozen.h>

#include <app/configuration.h>
#include <server/api.h>

#include <container.h>
#include <macro.h>

/***********************************************************************************************
 * CONTROLLERS HEADERS
 **********************************************************************************************/

#include <server/controllers/authorization.h>
#include <server/controllers/registration.h>
#include <server/controllers/server_control.h>
#include <server/controllers/welcome.h>

/***********************************************************************************************
 * STATIC FUNCTIONS DECLARATIONS
 **********************************************************************************************/

/**
 * @brief Run server.
 *
 * @param server Server object.
 */
static void run(p_server server);

/**
 * @brief Launch server.
 *
 * @param args Server arguments.
 */
static void *server_thread_run(void *args);

/**
 * @brief Server event handler.
 *
 * @param c Connection.
 * @param ev Event.
 * @param ev_data Event data.
 * @param fn_data Function data.
 */
static void server_event_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data);

/***********************************************************************************************
 * FUNCTIONS DEFINITIONS
 **********************************************************************************************/

p_server init_server(void) {
    p_container container = get_container(name_of(main));
    p_server server = (p_server)malloc(sizeof(server_t));
    if (!server)
        return NULL;

    p_configuration configuration = get_service_from_container(container, name_of(p_configuration));
    p_server_configuration server_cfg = configuration->server_configuration;

    server->configuration = server_cfg;
    server->run = run;
    server->server_state = SERVER_STATE_STOPPED;
    mg_mgr_init(&server->manager);

    return server;
}

void release_server(p_server server) {
    if (!server)
        return;

    if (&server->manager)
        mg_mgr_free(&server->manager);
    free(server);
    server = NULL;
}

/***********************************************************************************************
 * STATIC FUNCTIONS DEFINITIONS
 **********************************************************************************************/

static void run(p_server server) {
    pthread_create(&server->thread, NULL, server_thread_run, server);

    while (server->server_state == SERVER_STATE_STOPPED);
    pthread_join(server->thread, (void **){0});
}

static void *server_thread_run(void *args) {
    p_server server = (p_server)args;

    mg_http_listen(&server->manager, server->configuration->host, server_event_handler, NULL);
    mg_wakeup_init(&server->manager);
    server->server_state = SERVER_STATE_RUNNING;
    while (server->server_state == SERVER_STATE_RUNNING)
        mg_mgr_poll(&server->manager, 1000);

    pthread_exit(NULL);
    return NULL;
}

static void server_event_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    if (ev == MG_EV_OPEN && c->is_listening) {
        struct mg_http_message *hm = (struct mg_http_message *)ev_data;
    } else if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *)ev_data;
        if (mg_match(hm->uri, mg_str("/websocket"), NULL)) {
            mg_ws_upgrade(c, hm, NULL);
            c->data[0] = 'W';
        } else if (invoke_controller("/authorization", c, hm, authorization) ||
                   invoke_controller("/registration", c, hm, registration) ||
                   invoke_controller("/welcome", c, hm, welcome) ||
                   invoke_controller("/server_control", c, hm, server_control)) {
        } else {
            struct mg_http_serve_opts opts = {.root_dir = "."};
            mg_http_serve_dir(c, hm, &opts);
        }
    } else if (ev == MG_EV_WS_MSG) {
        struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
        mg_ws_send(c, wm->data.buf, wm->data.len, WEBSOCKET_OP_TEXT);
        mg_iobuf_del(&c->recv, 0, c->recv.len);
    } else if (ev == MG_EV_WAKEUP) {
        struct mg_str *data = (struct mg_str *) ev_data;
        for (struct mg_connection *wc = c->mgr->conns; wc != NULL; wc = wc->next) {
          if (wc->data[0] == 'W')
            mg_ws_send(wc, data->buf, data->len, WEBSOCKET_OP_TEXT);
        }
    }
}

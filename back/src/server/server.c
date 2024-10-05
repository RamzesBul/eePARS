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
#include <server/controllers/welcome.h>
#include <server/controllers/server_control.h>

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
    if (!server) return NULL;

    p_configuration configuration = get_service_from_container(container, name_of(p_configuration));
    p_server_configuration server_cfg = configuration->server_configuration;

    server->configuration = server_cfg;
    server->run = run;
    server->server_state = SERVER_STATE_STOPPED;
    mg_mgr_init(&server->manager);
    
    return server;
}

void release_server(p_server server) {
    if (!server) return;
    
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
}

static void *server_thread_run(void *args) {
    p_server server = (p_server)args;

    mg_http_listen(&server->manager, server->configuration->host, server_event_handler, NULL);
    server->server_state = SERVER_STATE_RUNNING;
    while (server->server_state) mg_mgr_poll(&server->manager, 1000);

    pthread_exit(NULL);
}

static void server_event_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    if (ev == MG_EV_HTTP_MSG) {
        p_container container = get_container(name_of(main));
        p_server server = get_service_from_container(container, name_of(p_server));
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;

        if (invoke_controller("/authorization", hm, authorization) ||
            invoke_controller("/welcome", hm, welcome) ||
            invoke_controller("/server_control", hm, server_control));
        else {
            struct mg_http_serve_opts opts = {.root_dir = "."};
            mg_http_serve_dir(c, hm, &opts);
        }
    }
}

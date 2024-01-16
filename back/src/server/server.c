#include <server/server.h>

#include <stdlib.h>

#include <client/client.h>
#include <cesanta/frozen.h>

#include <core/configuration.h>
#include <server/webapi/api.h>


/***********************************************************************************************
 * CONTROLLERS HEADERS
 **********************************************************************************************/
#include <server/controllers/authorization.h>
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

p_server init_server(p_server_configuration server_cfg) {
    p_server server = (p_server)malloc(sizeof(server_t));
    if (!server)
        return NULL;

    server->configuration = server_cfg;
    server->run = run;
    
    return server;
}

void release_server(p_server server) {
    if (server) {
        pthread_join(server->thread, &(void *) {0});        
        mg_mgr_free(&server->manager);
        free(server);        
    }
}


/***********************************************************************************************
 * STATIC FUNCTIONS DEFINITIONS
 **********************************************************************************************/

static void run(p_server server) {
    pthread_create(&server->thread, NULL, server_thread_run, server);
}

static void *server_thread_run(void *args) {
    p_server server = (p_server)args;

    mg_mgr_init(&server->manager);
    mg_http_listen(&server->manager, server->configuration->host, server_event_handler, server);
    
    for (;;) mg_mgr_poll(&server->manager, 1000);

    pthread_exit(NULL);
}

static void server_event_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    if (ev == MG_EV_HTTP_MSG) {
        p_server server = (p_server) fn_data;
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;

        if (invoke_controller("/authorization", hm, server, authorization) ||
            invoke_controller("/welcome", hm, server, welcome));
        else {
            struct mg_http_serve_opts opts = {.root_dir = "."};
            mg_http_serve_dir(c, hm, &opts);
        }
    }
}

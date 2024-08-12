#include <server/controllers/server_control.h>

#include <server/server.h>
#include <client/client.h>

#include <container.h>
#include <macro.h>

/***********************************************************************************************
 * STATIC FUNCTIONS DECLARATIONS
 **********************************************************************************************/

int recieve_signal(struct mg_http_message *hm, const char *signal);

/***********************************************************************************************
 * FUNCTIONS DEFINITIONS
 **********************************************************************************************/

void server_control(struct mg_http_message *hm) {
    p_server server = get_service_from_global_container(name_of(p_server));

    if (!hm->query.len)
        mg_http_reply(server->manager.conns, 400, server->configuration->cors_policy, "%s", "No control message.");

    int is_stop_signal = recieve_signal(hm, "server_stop");

    if (is_stop_signal) {
        mg_http_reply(server->manager.conns, 200, server->configuration->cors_policy, "Server is stopping...");
        server->server_state = SERVER_STATE_STOPPED;
    }
}

/***********************************************************************************************
 * STATIC FUNCTIONS DEFINITIONS
 **********************************************************************************************/

int recieve_signal(struct mg_http_message *hm, const char *signal) {
    return strstr(hm->query.ptr, signal) ? 1 : 0;
}
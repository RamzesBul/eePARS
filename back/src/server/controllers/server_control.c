#include <server/controllers/server_control.h>

#include <server/server.h>
#include <server/api.h>
#include <client/api.h>
#include <user/user.h>

#include <container.h>
#include <macro.h>


/***********************************************************************************************
* FUNCTIONS DEFINITIONS
**********************************************************************************************/

void *server_control(struct mg_http_message *hm) {
    p_container container = get_container(name_of(main));
    p_server server = get_service_from_container(container, name_of(p_server));

    p_user user = get_service_from_container_with_args(container, name_of(p_user), hm);

    if (!user || (user && !user->role != 0))
        mg_http_reply(server->manager.conns, 403, server->configuration->cors_policy, "You are not allowed to access this method.");

    if (!hm->query.len)
        mg_http_reply(server->manager.conns, 400, server->configuration->cors_policy, "%s", "No control message.");

    const char *signal = from_query(hm, name_of(signal)); 

    if (strcmp(signal, "server_stop") == 0) {
        mg_http_reply(server->manager.conns, 200, server->configuration->cors_policy, "Server is stopping...");
        server->server_state = SERVER_STATE_STOPPED;
    }

    if (strcmp(signal, "request_get") == 0) {
        const char *url = from_query(hm, name_of(url)); 
        if (url) {
            const char *result = request_get(url);
            if (result) {
                mg_http_reply(server->manager.conns, 200, server->configuration->cors_policy, result);
            } else {
                mg_http_reply(server->manager.conns, 200, server->configuration->cors_policy, "No response from specified url.");
            }
            free(url);
        } else {
            mg_http_reply(server->manager.conns, 200, server->configuration->cors_policy, "You should specify the url!");
        }
    }

    return NULL;
}
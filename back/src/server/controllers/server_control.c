#include <server/controllers/server_control.h>

#include <server/server.h>
#include <client/client.h>
#include <client/api.h>

#include <container.h>
#include <macro.h>

/***********************************************************************************************
 * STATIC FUNCTIONS DECLARATIONS
 **********************************************************************************************/

static int recieve_signal(struct mg_http_message *hm, const char *signal);

static char *recieve_url(struct mg_http_message *hm);

/***********************************************************************************************
 * FUNCTIONS DEFINITIONS
 **********************************************************************************************/

void server_control(struct mg_http_message *hm) {
    p_container container = get_container(name_of(main));
    p_server server = get_service_from_container(container, name_of(p_server));

    if (!hm->query.len)
        mg_http_reply(server->manager.conns, 400, server->configuration->cors_policy, "%s", "No control message.");

    int is_stop_signal = recieve_signal(hm, "server_stop");
    if (is_stop_signal) {
        mg_http_reply(server->manager.conns, 200, server->configuration->cors_policy, "Server is stopping...");
        server->server_state = SERVER_STATE_STOPPED;
    }

    int is_request_get = recieve_signal(hm, "request_get");
    if (is_request_get) {
        const char *url = recieve_url(hm);
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
}

/***********************************************************************************************
 * STATIC FUNCTIONS DEFINITIONS
 **********************************************************************************************/

static int recieve_signal(struct mg_http_message *hm, const char *signal) {
    return strstr(hm->query.ptr, signal) ? 1 : 0;
}

static char *recieve_url(struct mg_http_message *hm) {
    const char *urlStart = strstr(hm->query.ptr, "http");
    const char *urlEnd = strstr(hm->query.ptr, " HTTP");

    if (urlStart && urlEnd) {
        char *url = (char *)malloc(urlEnd - urlStart + 1);
        memcpy(url, urlStart, urlEnd - urlStart);
        url[urlEnd - urlStart] = '\0';

        return url;
    }
    
    return NULL;
}
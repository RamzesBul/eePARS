#include <server/controllers/welcome.h>

#include <server/server.h>
#include <client/client.h>
#include <app/application.h>


/***********************************************************************************************
 * FUNCTIONS DEFINITIONS
 **********************************************************************************************/

void welcome(struct mg_http_message *hm, p_server server) {
    p_client client = get_app_client_instance();
    char *response = client->configuration->vk_api_auth_uri;

    mg_http_reply(server->manager.conns, 200, server->configuration->cors_policy, "%s", response);
}
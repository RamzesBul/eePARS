#include <server/controllers/welcome.h>

#include <server/server.h>
#include <client/client.h>

#include <container.h>
#include <macro.h>

/***********************************************************************************************
 * FUNCTIONS DEFINITIONS
 **********************************************************************************************/

void welcome(struct mg_http_message *hm) {
    p_container container = get_container(name_of(main));

    p_server server = get_service_from_container(container, name_of(p_server));
    p_client client = get_service_from_container(container, name_of(p_client));

    char *response = client->configuration->vk_cfg->url;

    mg_http_reply(server->manager.conns, 200, server->configuration->cors_policy, "%s", response);
}
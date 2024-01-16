#include <server/controllers/authorization.h>

#include <server/server.h>
#include <client/client.h>
#include <client/api.h>
#include <app/application.h>


/***********************************************************************************************
 * FUNCTIONS DEFINITIONS
 **********************************************************************************************/

void authorization(struct mg_http_message *hm, p_server server) {
    char *access_token_start = strstr(hm->query.ptr, "access_token=");
    char *access_token_end = strstr(hm->query.ptr, " HTTP");
    int access_token_length = (int)((long long)access_token_end - (long long)access_token_start);

    if (!access_token_start || !access_token_end || !access_token_length) {
        return;
    }

    char token[256];
    strncpy(token, access_token_start, access_token_length);
    token[access_token_length] = 0;

    char request_buff[512];
    sprintf(request_buff, "https://api.vk.com/method/account.getProfileInfo?%s&v=5.154", token);

    p_client client = get_app_client_instance();
    char *response = request_get(client, request_buff);
    mg_http_reply(server->manager.conns, 200, server->configuration->cors_policy, "%s", response);
}
#include <server/api.h>


/***********************************************************************************************
 * FUNCTIONS DEFINITION
 **********************************************************************************************/

int invoke_controller(const char *url, struct mg_http_message *hm, p_server server, 
                      void (*controller)(struct mg_http_message *hm, p_server server)) {
    if (mg_http_match_uri(hm, url)) {
        controller(hm, server);
        return 1;
    }

    return 0;
}
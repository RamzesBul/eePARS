#include <server/api.h>


/***********************************************************************************************
 * FUNCTIONS DEFINITION
 **********************************************************************************************/

int invoke_controller(const char *url, 
                      struct mg_http_message *hm,
                      void (*controller)(struct mg_http_message *hm)) {
    if (mg_http_match_uri(hm, url)) {
        controller(hm);
        return 1;
    }
    return 0;
}
#ifndef EEPARS_VKCLIENT_H
#define EEPARS_VKCLIENT_H

#include <mongoose.h>

static const char *s_url = "https://oauth.vk.com/authorize?client_id=51737169&display=page&redirect_uri=https://oauth.vk.com/blank.html&scope=friends&response_type=token&v=5.131&state=123456"; // Source URL

/**
 * @brief Send request to server.
 * 
 * @param url URL to send request to.
 */
void request_get(const char *url);

#endif //EEPARS_VKCLIENT_H
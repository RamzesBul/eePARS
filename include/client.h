#ifndef EEPARS_VKCLIENT_H
#define EEPARS_VKCLIENT_H

#include <mongoose.h>

/**
 * @brief Send request to server.
 * 
 * @param url URL to send request to.
 * @param callback_function Callback function to handle response.
 */
void request_get(const char *url, mg_event_handler_t callback_function);

#endif //EEPARS_VKCLIENT_H
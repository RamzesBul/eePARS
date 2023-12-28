/**
 * @file client.h
 * @brief Client to APIs.
 * @date 2023-09-09
 */

#ifndef EEPARS_VKCLIENT_H
#define EEPARS_VKCLIENT_H

#include <cesanta/mongoose.h>

/**
 * Send GET request.
 * @param url - URL.
 * @return Response.
 */
char *request_get(const char *url);

#endif //EEPARS_VKCLIENT_H
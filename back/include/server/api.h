#ifndef EEPARS_SERVER_API_H
#define EEPARS_SERVER_API_H

#include <cesanta/mongoose.h>

/*********************************************************************************************
 * FUNCTIONS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Invoke controller.
 *
 * @param url URL.
 * @param hm HTTP message.
 * @param controller Controller.
 */
extern int invoke_controller(const char *url, struct mg_connection *c,
                             struct mg_http_message *hm,
                             void *(*controller)(struct mg_http_message *hm));

/**
 * @brief Receive parameter from URI.
 *
 * @param hm HTTP message.
 * @param name Parameter name.
 *
 * @return Parameter value.
 */
extern char *from_query(struct mg_http_message *hm, const char *name);

/**
 * @brief Receive data from HTTP body.
 *
 * @param hm HTTP message.
 * @param path Path to param.
 *
 * @return Parameter value.
 */
extern char *from_body(struct mg_http_message *hm, const char *path);

/**
 * @brief Receive header from HTTP message.
 *
 * @param hm HTTP message.
 * @param name Header parametere name.
 *
 * @return Header parameter value.
 */
extern char *from_header(struct mg_http_message *hm, const char *name);

#endif // EEPARS_SERVER_API_H
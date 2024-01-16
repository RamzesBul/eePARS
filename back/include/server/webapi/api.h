#ifndef EEPARS_SERVER_API_H
#define EEPARS_SERVER_API_H

#include <cesanta/mongoose.h>

#include <server/server.h>


/*********************************************************************************************
 * FUNCTIONS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Invoke controller.
 *
 * @param url URL.
 * @param hm HTTP message.
 * @param server Server.
 * @param controller Controller.
 */
extern int invoke_controller(const char *url,
                      struct mg_http_message *hm,
                      p_server server,
                      void (*controller)(struct mg_http_message *hm, p_server server));

#endif // EEPARS_SERVER_API_H
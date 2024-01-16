#ifndef CONTROLLERS_WELCOME_H
#define CONTROLLERS_WELCOME_H

#include <cesanta/mongoose.h>

#include <server/server.h>


/*********************************************************************************************
 * FUNCTIONS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Welcome controller.
 * 
 * @param hm HTTP message.
 * @param server Server object <p_server>.
 */
extern void welcome(struct mg_http_message *hm, p_server server);

#endif // CONTROLLERS_WELCOME_H
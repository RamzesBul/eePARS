#ifndef CONTROLLERS_AUTHORIZATION_H
#define CONTROLLERS_AUTHORIZATION_H

#include <cesanta/mongoose.h>

#include <server/server.h>


/*********************************************************************************************
 * FUNCTIONS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Authorization controller.
 * 
 * @param hm HTTP message.
 * @param server Server object <p_server>.
 */
extern void authorization(struct mg_http_message *hm, p_server server);

#endif // CONTROLLERS_AUTHORIZATION_H
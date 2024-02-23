#ifndef CONTROLLERS_SERVER_CONTROL_H
#define CONTROLLERS_SERVER_CONTROL_H

#include <cesanta/mongoose.h>

/*********************************************************************************************
 * FUNCTIONS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Server control controller.
 * 
 * @param hm HTTP message.
 */
extern void server_control(struct mg_http_message *hm);

#endif // CONTROLLERS_SERVER_CONTROL_H
#ifndef CONTROLLERS_AUTHORIZATION_H
#define CONTROLLERS_AUTHORIZATION_H

#include <cesanta/mongoose.h>

/*********************************************************************************************
 * FUNCTIONS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Authorization controller.
 *
 * @param hm HTTP message.
 */
extern void *authorization(struct mg_http_message *hm);

#endif // CONTROLLERS_AUTHORIZATION_H
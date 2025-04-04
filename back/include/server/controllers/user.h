#ifndef CONTROLLERS_USER_H
#define CONTROLLERS_USER_H

#include <cesanta/mongoose.h>

/*********************************************************************************************
 * FUNCTIONS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Return a string containing the user information.
 *
 * @param hm HTTP message.
 */
extern void *getUserInfo(struct mg_http_message *hm);

#endif // CONTROLLERS_USER_H
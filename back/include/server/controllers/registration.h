#ifndef CONTROLLERS_REGISTRATION_H
#define CONTROLLERS_REGISTRATION_H

#include <cesanta/mongoose.h>

/*********************************************************************************************
 * FUNCTIONS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Registartion controller.
 *
 * @param hm HTTP message.
 */
extern void *registration(struct mg_http_message *hm);

#endif // CONTROLLERS_REGISTRATION_H
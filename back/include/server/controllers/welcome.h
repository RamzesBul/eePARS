#ifndef CONTROLLERS_WELCOME_H
#define CONTROLLERS_WELCOME_H

#include <cesanta/mongoose.h>

/*********************************************************************************************
 * FUNCTIONS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Welcome controller.
 * 
 * @param hm HTTP message.
 */
extern void welcome(struct mg_http_message *hm);

#endif // CONTROLLERS_WELCOME_H
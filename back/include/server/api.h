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
extern int invoke_controller(const char *url,
                      struct mg_http_message *hm,
                      void (*controller)(struct mg_http_message *hm));

#endif // EEPARS_SERVER_API_H
#ifndef EEPARS_SERVER_H
#define EEPARS_SERVER_H

#include <mongoose.h>

/**
 * @brief Server listener arguments.
 */
typedef struct server_lister_args {
    char *port; // Port to listen on.
    mg_event_handler_t callback_function; // Callback function for server.
} sla_t;

/**
 * @brief Server listener thread.
 * 
 * @param args Arguments (see \b server_lister_args).
 */
void *server_listener(void *args);

#endif // EEPARS_SERVER_H
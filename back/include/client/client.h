#ifndef EEPARS_CLIENT_H
#define EEPARS_CLIENT_H

#include <cesanta/mongoose.h>

#include <app/configuration.h>

/*********************************************************************************************
 * STRUCTS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Client.
 */
typedef struct client_s {
    p_client_configuration configuration; // Configuration.
    struct mg_mgr manager;                // Mongoose manager.
    struct mg_connection *connection;     // Mongoose connection.
    void (*run)(struct client_s *client); // Run client.
} client_t, *p_client;

/*********************************************************************************************
 * FUNCTIONS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Initialize client.
 * 
 * @return Client object.
 */
extern p_client init_client();

/**
 * @brief Release client.
 * 
 * @param client Client object.
 */
extern void release_client(p_client client);

#endif // EEPARS_CLIENT_H
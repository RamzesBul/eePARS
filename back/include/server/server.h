#ifndef EEPARS_SERVER_H
#define EEPARS_SERVER_H

#include <pthread.h>

#include <cesanta/mongoose.h>

#include <core/configuration.h>


/*********************************************************************************************
 * STRUCTS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Server.
 */
typedef struct server_s {
    p_server_configuration configuration; // Server configuration.
    struct mg_mgr manager;                // Mongoose manager.
    pthread_t thread;                     // Server thread.
    void (*run)(struct server_s *server); // Run server.
} server_t, *p_server;


/*********************************************************************************************
 * FUNCTIONS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Initialize server.
 * 
 * @param server_cfg Server configutation.
 * @return Server object.
 */
extern p_server init_server(p_server_configuration server_cfg);

/**
 * @brief Release server.
 * 
 * @param server Server.
 */
extern void release_server(p_server server);

#endif // EEPARS_SERVER_H
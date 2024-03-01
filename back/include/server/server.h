#ifndef EEPARS_SERVER_H
#define EEPARS_SERVER_H

#include <pthread.h>

#include <cesanta/mongoose.h>

#include <app/configuration.h>

/*********************************************************************************************
 * STRUCTS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Server state configuration.
 */
typedef enum server_state_s {
    SERVER_STATE_STOPPED = 0, // Server stopped.
    SERVER_STATE_RUNNING = 1  // Server running.
} server_state_t, *p_server_state;

/**
 * @brief Server.
 */
typedef struct server_s {
    p_server_configuration configuration; // Server configuration.
    struct mg_mgr manager;                // Mongoose manager.
    pthread_t thread;                     // Server thread.
    server_state_t server_state;          // Server state.
    void (*run)(struct server_s *server); // Run server.
} server_t, *p_server;

/*********************************************************************************************
 * FUNCTIONS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Initialize server.
 * 
 * @return Server object.
 */
extern p_server init_server();

/**
 * @brief Release server.
 * 
 * @param server Server.
 */
extern void release_server(p_server server);

#endif // EEPARS_SERVER_H
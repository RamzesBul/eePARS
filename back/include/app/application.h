#ifndef EEPARS_APPLICATION_H
#define EEPARS_APPLICATION_H

#include <app/configuration.h>

#include <client/client.h>
#include <server/server.h>

/*********************************************************************************************
 * STRUCTS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Application.
 */
typedef struct application_s {
    struct application_s *(*add_server)(); // Add server.
    struct application_s *(*add_client)(); // Add client.
    void (*run)(); // Run application.

    p_configuration cfg; // Configuration.
    p_server server;     // Server.
    p_client client;     // Client.
} application_t, *p_application;

/*********************************************************************************************
 * FUNCTIONS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Initialize application.
 *
 * @return Application object.
 */
extern p_application init_application(void);

/**
 * @brief Release application.
 * 
 * @param app Application object.
 */
extern void release_application(p_application app);

#endif // EEPARS_APPLICATION_H
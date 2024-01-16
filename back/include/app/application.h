#ifndef EEPARS_APPLICATION_H
#define EEPARS_APPLICATION_H

#include <app/service.h>
#include <client/client.h>
#include <server/server.h>


/*********************************************************************************************
 * STRUCTS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Application.
 */
typedef struct application_s {
    struct application_s *(*add_server)(struct application_s * app, p_services services); // Add server.
    struct application_s *(*add_client)(struct application_s * app, p_services services); // Add client.
    void (*run)(struct application_s *app); // Run application.
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

/**
 * @brief Returns the application @b client object.
 * 
 * @return Client object.
 */
extern p_client get_app_client_instance();

/**
 * @brief Returns the application @b server object.
 * 
 * @return Server object.
 */
extern p_server get_app_server_instance();

#endif // EEPARS_APPLICATION_H
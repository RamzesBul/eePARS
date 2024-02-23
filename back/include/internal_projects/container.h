/**
 * @file container.h
 * @author chcp (cmewhou@yandex.ru)
 * @brief Common container for global access to application services.
 * @version 1.0
 * @date 2024-02-23
 *
 * @copyright Copyright (c) 2024
 */

#ifndef IPEE_CONTAINER_H
#define IPEE_CONTAINER_H

#include <internal_projects/macro.h>

/*********************************************************************************************
 * STRUCTS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Element lifetime type.
 *
 * @details
 * Defines the element lifetime.
 * Singleton - one instance of service in the application.
 * Transient - new instance of service on each new request.
 */
typedef enum service_type_e {
    SERVICE_TYPE_SINGLETON = 0,
    SERVICE_TYPE_TRANSIENT = 1,
} service_type_t, *p_service_type;

/***********************************************************************************************
 * FUNCTION TYPEDEFS
 **********************************************************************************************/

/**
 * @brief Callback function for initializing or releasing services.
 *
 * @param args Arguments for callback function.
 */
typedef void *(*container_callback_function)(void *args);

/*********************************************************************************************
 * FUNCTIONS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Initialize container.
 *
 * @details
 * Create application container.
 * To add services to it use "add_service_to_container" function.
 */
extern void init_container();

/**
 * @brief Release container.
 *
 * @details
 * Release container and all services.
 * Invokes callback function to release resources of each service.
 */
extern void release_container();

/**
 * @brief Add new service to container.
 *
 * @details
 * Add new service to application container.
 * If service with specified key exists it will be replaced.
 * 
 * @param type Lifetime type.
 * @param key Service identifier.
 * @param initial_callback Initial callback function.
 * @param release_callback Release callback function.
 * @param args Arguments.
 * 
 * @warning
 * When replacing service, its resources will not be released.
 * All resource will be release at the "release_container" function call.
 */
extern void add_service_to_container(const service_type_t type, char *key,
                                     container_callback_function initial_callback,
                                     container_callback_function release_callback,
                                     void *args);

/**
 * @brief Get service instance from container.
 *
 * @details
 * Returns specified service instance if it is defined.
 * 
 * @param key Service identifier.
 * @return Service instance.
 */
extern void *get_service_from_container(char *key);

#endif // IPEE_CONTAINER_H
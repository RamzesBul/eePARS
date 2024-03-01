/**
 * @file container.h
 * @author chcp (cmewhou@yandex.ru)
 * @brief Common container for global access to application services.
 * @version 1.1
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
 * Glblvalue - global value.
 */
typedef enum service_type_e {
    SERVICE_TYPE_SINGLETON = 0, // One instance of service in the application.
    SERVICE_TYPE_TRANSIENT = 1, // New instance of service on each new request.
    SERVICE_TYPE_GLBLVALUE = 2, // Global value.
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
 * @brief Add new global value to container.
 * 
 * @param key Global value identifier.
 * @param value Global value.
 * @param release_callback Release callback function.
 * 
 * @details
 * Add new global value to application container.
 * If global value with specified key exists it will be replaced.
 */
extern void add_glblvalue_to_container(char *key, void *value, container_callback_function release_callback);

/**
 * @brief Add new singleton to container.
 *
 * @details
 * Add new singleton to application container.
 * If singleton with specified key exists it will be replaced.
 * 
 * @param key Singleton identifier.
 * @param initial_callback Initial callback function.
 * @param release_callback Release callback function.
 */
extern void add_singleton_to_container(char *key, container_callback_function initial_callback, container_callback_function release_callback);

/**
 * @brief Add new transient to container.
 *
 * @details
 * Add new transient to application container.
 * If transient with specified key exists it will be replaced.
 * 
 * @param key Transient identifier.
 * @param initial_callback Initial callback function.
 * @param release_callback Release callback function.
 */
extern void add_transient_to_container(char *key, container_callback_function initial_callback, container_callback_function release_callback);

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
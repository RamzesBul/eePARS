#ifndef EEPARS_SERVICE_H
#define EEPARS_SERVICE_H

#include <core/server.h>
#include <core/cred.h>

/**
 * @brief Services.
 */
typedef struct {
    p_credentials credentials;
    p_server server;
} services_t, *p_services;

/**
 * @brief Register services.
 */
p_services register_services(void);

/**
 * @brief Release services.
 */
void release_services(p_services this);

#endif //EEPARS_SERVICE_H

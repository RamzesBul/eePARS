#ifndef EEPARS_SERVICE_H
#define EEPARS_SERVICE_H

#include <core/configuration.h>


/*********************************************************************************************
 * STRUCTS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Services.
 */
typedef struct services_s {
    p_configuration configuration; // Configuration.
    
    void (*add_configuration)(struct services_s *services, p_configuration cfg); // Add configuration.
} services_t, *p_services;


/*********************************************************************************************
 * FUNCTIONS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Register services.
 * 
 * @return Services object.
 */
extern p_services init_services(void);

/**
 * @brief Release services.
 * 
 * @param services Services.
 */
extern void release_services(p_services services);

#endif // EEPARS_SERVICE_H

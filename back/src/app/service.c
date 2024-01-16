#include <app/service.h>

#include <stdlib.h>
#include <stdio.h>


/***********************************************************************************************
 * STATIC FUNCTIONS DECLARATIONS
 **********************************************************************************************/

/**
 * @brief Add configuration.
 * 
 * @param services Services object.
 * @param cfg Configuration object.
 */
static void add_configuration(p_services services, p_configuration cfg);


/***********************************************************************************************
 * FUNCTIONS DEFINITIONS
 **********************************************************************************************/

p_services init_services(void) {
    p_services services = (p_services)malloc(sizeof(services_t));

    services->add_configuration = add_configuration;
    
    return services;
}

void release_services(p_services services) {
    if (services)
        free(services);
}


/***********************************************************************************************
 * STATIC FUNCTIONS DEFINITIONS
 **********************************************************************************************/

static void add_configuration(p_services services, p_configuration cfg) {
    if (services && cfg)
        services->configuration = cfg;
}
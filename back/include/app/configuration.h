#ifndef EEPARS_CONFIGURATION_H
#define EEPARS_CONFIGURATION_H

/*********************************************************************************************
 * STRUCTS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Client configuration.
 *
 * @details
 * It contains configurations to connect to the others services.
 */
typedef struct client_configuration_s {}
client_configuration_t, *p_client_configuration;

/**
 * @brief Server configuration.
 *
 * @details
 * It contains the server policies and settings.
 */
typedef struct server_configuration_s {
    char *host;                   // Server host address.
    char *cors_policy;            // CORS policy.
    char *postrges_db_connection; // Postrges DB connection.
} server_configuration_t, *p_server_configuration;

/**
 * @brief Configuration.
 *
 * @details
 * It contains the client and server configurations.
 */
typedef struct configuration_s {
    p_client_configuration client_configuration; // Client configuration.
    p_server_configuration server_configuration; // Server configuration.
} configuration_t, *p_configuration;

/*********************************************************************************************
 * FUNCTIONS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Initialize configuration.
 *
 * @return Configuration object.
 */
extern p_configuration init_configuration(void);

/**
 * @brief Release configuration.
 *
 * @param cfg Configuration object.
 */
extern void release_configuration(p_configuration cfg);

#endif // EEPARS_CONFIGURATION_H
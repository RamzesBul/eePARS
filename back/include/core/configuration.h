#ifndef EEPARS_CONFIGURATION_H
#define EEPARS_CONFIGURATION_H


/*********************************************************************************************
 * STRUCTS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief VK API configuration.
 */
typedef struct client_configuration_s {
    char *vk_api_auth_uri; // Authorize VK API URI.
    char *vk_api_cert;     // VK API certificate.
} client_configuration_t, *p_client_configuration;

/**
 * @brief Server configuration.
 */
typedef struct server_configuration_s {
    char *host;        // Server host.
    char *cors_policy; // CORS policy.
} server_configuration_t, *p_server_configuration;

/**
 * @brief Configuration.
 */
typedef struct configuration_s {
    p_client_configuration client_configuration; // Client configuration.
    p_server_configuration server_configuration; // Server configuration.

    struct configuration_s * (*add_client_cfg)(struct configuration_s *cfg); // Add VK API configuration.
    struct configuration_s * (*add_server_cfg)(struct configuration_s *cfg); // Add server configuration.
} configuration_t, *p_configuration;


/*********************************************************************************************
 * FUNCTIONS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Initialize configuration.
 * 
 * @param path Path to configuration file.
 * @return Configuration object.
 */
extern p_configuration init_configuration(const char *path);

/**
 * @brief Release configuration.
 * @param cfg Configuration object.
 */
extern void release_configuration(p_configuration cfg);

#endif // EEPARS_CONFIGURATION_H
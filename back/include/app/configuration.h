#ifndef EEPARS_CONFIGURATION_H
#define EEPARS_CONFIGURATION_H

/*********************************************************************************************
 * STRUCTS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief VK API client configuration.
 * 
 * @details
 * It contains the target URL and the certificate to be used for the connection to the VK API.
 * url - Target URL.
 * cert - Certificate. 
 */
typedef struct vk_client_configuration_s {
    char *url;  // Target URL.
    char *cert; // Certificate.
} vk_client_configuration_t, *p_vk_client_configuration;

/**
 * @brief Client configuration.
 * 
 * @details
 * It contains configurations to connect to the others services.
 */
typedef struct client_configuration_s {
    p_vk_client_configuration vk_cfg; // VK API configuration.
} client_configuration_t, *p_client_configuration;

/**
 * @brief Server configuration.
 * 
 * @details
 * It contains the server policies and settings.
 */
typedef struct server_configuration_s {
    char *host;        // Server host.
    char *cors_policy; // CORS policy.
} server_configuration_t, *p_server_configuration;

/**
 * @brief Configuration.
 * 
 * @details
 * It contains the client and server configurations.
 */
typedef struct configuration_s {
    p_client_configuration client_configuration;                // Client configuration.
    p_server_configuration server_configuration;                // Server configuration.

    struct configuration_s *(*open_cfg_file)(const char *path); // Open configuration file.
    struct configuration_s *(*add_client_cfg)();                // Add VK API configuration.
    struct configuration_s *(*add_server_cfg)();                // Add server configuration.
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
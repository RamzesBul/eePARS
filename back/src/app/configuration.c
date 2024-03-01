#include <app/configuration.h>

#include <stdlib.h>
#include <string.h>

#include <cesanta/frozen.h>

#include <container.h>

/***********************************************************************************************
 * SCOPED STRUCTS DECLARATIONS
 **********************************************************************************************/

/**
 * @brief Wrapped configuration.
 * 
 * @details
 * This structure is used to wrap configuration object.
 * It contains raw JSON data, client and server configurations.
 */
typedef struct wrap_configuration_s {
    p_client_configuration client_configuration;                // Client configuration.
    p_server_configuration server_configuration;                // Server configuration.

    struct configuration_s *(*open_cfg_file)(const char *path); // Open configuration file.
    struct configuration_s *(*add_client_cfg)();                // Add VK API configuration.
    struct configuration_s *(*add_server_cfg)();                // Add server configuration.

    char *raw_json_data;                                        // Raw data from configuration file.
    int raw_json_data_size;                                     // Raw data size.
} wrap_configuration_t, *p_wrap_configuration;

/***********************************************************************************************
 * STATIC FUNCTIONS DECLARATIONS
 **********************************************************************************************/

/**
 * @brief Open configuration file.
 * 
 * @param path Path to configuration file.
 */
static p_configuration open_cfg_file(const char *path);

/**
 * @brief Read raw JSON data from file.
 *
 * @param wrapped_cfg Wrapped configuration object.
 * @param path Path to configuration file.
 */
static void read_raw_json_data(p_wrap_configuration wrapped_cfg, const char *path);

/**
 * @brief Add client configuration.
 *
 * @return Configuration object.
 */
static p_configuration add_client_cfg();

/**
 * @brief Add server configuration.
 *
 * @return Configuration object.
 */
static p_configuration add_server_cfg();

/**
 * @brief Read client configuration.
 *
 * @param wrapped_cfg Wrapped configuration object.
 * @return Client configuration object.
 */
static p_client_configuration read_client_cfg(p_wrap_configuration wrapped_cfg);

/**
 * @brief Read server configuration.
 *
 * @param wrapped_cfg Wrapped configuration object.
 * @return Server configuration object.
 */
static p_server_configuration read_server_cfg(p_wrap_configuration wrapped_cfg);

/**
 * @brief Release raw JSON data.
 *
 * @param wrapped_cfg Wrapped configuration object.
 */
static void release_raw_json_data(p_wrap_configuration wrapped_cfg);

/**
 * @brief Release VK API configuration.
 *
 * @param wrapped_cfg Wrapped configuration object.
 */
static void release_client_cfg(p_wrap_configuration wrapped_cfg);

/**
 * @brief Release server configuration.
 *
 * @param wrapped_cfg Wrapped configuration object.
 */
static void release_server_cfg(p_wrap_configuration wrapped_cfg);

/***********************************************************************************************
 * FUNCTIONS DEFINITIONS
 **********************************************************************************************/

p_configuration init_configuration(void) {
    p_wrap_configuration wrapped_cfg = (p_wrap_configuration)malloc(sizeof(wrap_configuration_t));

    wrapped_cfg->client_configuration = NULL;
    wrapped_cfg->server_configuration = NULL;
    wrapped_cfg->open_cfg_file = open_cfg_file;
    wrapped_cfg->add_client_cfg = add_client_cfg;
    wrapped_cfg->add_server_cfg = add_server_cfg;
    wrapped_cfg->raw_json_data = NULL;
    wrapped_cfg->raw_json_data_size = 0;

    return (p_configuration)wrapped_cfg;
}

p_configuration open_cfg_file(const char *path) {
    p_wrap_configuration wrapped_cfg = get_service_from_container(name_of(p_configuration));

    read_raw_json_data(wrapped_cfg, path);

    return (p_configuration)wrapped_cfg;
}

void release_configuration(p_configuration cfg) {
    p_wrap_configuration wrapped_cfg = (p_wrap_configuration)cfg;
    if (!wrapped_cfg) return;

    if (wrapped_cfg->client_configuration) release_client_cfg(wrapped_cfg);
    if (wrapped_cfg->server_configuration) release_server_cfg(wrapped_cfg);
    if (wrapped_cfg->raw_json_data) free(wrapped_cfg->raw_json_data);
    free(wrapped_cfg);
    
    wrapped_cfg = NULL;
}

/***********************************************************************************************
 * STATIC FUNCTIONS DEFINITIONS
 **********************************************************************************************/

static void read_raw_json_data(p_wrap_configuration wrapped_cfg, const char *path) {
    wrapped_cfg->raw_json_data = json_fread(path);
    wrapped_cfg->raw_json_data_size = strlen(wrapped_cfg->raw_json_data);
}

static p_configuration add_client_cfg() {
    p_wrap_configuration wrapped_cfg = get_service_from_container(name_of(p_configuration));

    wrapped_cfg->client_configuration = read_client_cfg(wrapped_cfg);
    return (p_configuration)wrapped_cfg;
}

static p_configuration add_server_cfg() {
    p_wrap_configuration wrapped_cfg = get_service_from_container(name_of(p_configuration));

    wrapped_cfg->server_configuration = read_server_cfg(wrapped_cfg);
    return (p_configuration)wrapped_cfg;
}

static p_client_configuration read_client_cfg(p_wrap_configuration wrapped_cfg) {
    p_client_configuration client_cfg = (p_client_configuration)malloc(sizeof(client_configuration_t));
    if (!client_cfg) return NULL;
    client_cfg->vk_cfg = NULL;

    const char *path;
    json_scanf(wrapped_cfg->raw_json_data, wrapped_cfg->raw_json_data_size, "{CLIENT_SETTINGS:%Q}", &path);
    const char *json_client_data = json_fread(path);
    const int json_client_data_size = strlen(json_client_data);

    client_cfg->vk_cfg = (p_vk_client_configuration)malloc(sizeof(vk_client_configuration_t));
    if (client_cfg->vk_cfg) {
        client_cfg->vk_cfg->url = NULL;
        client_cfg->vk_cfg->cert = NULL;

        json_scanf(json_client_data, json_client_data_size, "{VK_API:{AUTHORIZE_URI:%Q}}", &client_cfg->vk_cfg->url);
        json_scanf(json_client_data, json_client_data_size, "{VK_API:{ACCESS_CERTIFICATE:%Q}}", &client_cfg->vk_cfg->cert);
    }
    
    return client_cfg;
}

static p_server_configuration read_server_cfg(p_wrap_configuration wrapped_cfg) {
    p_server_configuration server_cfg = (p_server_configuration)malloc(sizeof(server_configuration_t));
    if (!server_cfg) return NULL;
    server_cfg->host = NULL;
    server_cfg->cors_policy = NULL;

    const char *path;
    json_scanf(wrapped_cfg->raw_json_data, wrapped_cfg->raw_json_data_size, "{SERVER_SETTINGS:%Q}", &path);
    const char *json_server_data = json_fread(path);
    const int json_server_data_size = strlen(json_server_data);

    json_scanf(json_server_data, json_server_data_size, "{HOST_URI:%Q}", &server_cfg->host);
    json_scanf(json_server_data, json_server_data_size, "{CORS_POLICY:%Q}", &server_cfg->cors_policy);

    return server_cfg;
}

static void release_client_cfg(p_wrap_configuration wrapped_cfg) {
    if (!wrapped_cfg) return;

    p_client_configuration client_cfg = (p_client_configuration)wrapped_cfg->client_configuration;
    if (client_cfg) {
        if (client_cfg->vk_cfg) {
            if (client_cfg->vk_cfg->url) free(client_cfg->vk_cfg->url);
            if (client_cfg->vk_cfg->cert) free(client_cfg->vk_cfg->cert);
            free(client_cfg->vk_cfg);

            client_cfg->vk_cfg = NULL;
        }
        free(client_cfg);

        client_cfg = NULL;
    }
}

static void release_server_cfg(p_wrap_configuration wrapped_cfg) {
    if (!wrapped_cfg) return;
    
    p_server_configuration server_cfg = (p_server_configuration)wrapped_cfg->server_configuration;
    if (server_cfg) {
        if (server_cfg->host) free(server_cfg->host);
        if (server_cfg->cors_policy) free(server_cfg->cors_policy);
        free(server_cfg);

        server_cfg = NULL;
    }
}
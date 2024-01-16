#include <core/configuration.h>

#include <stdlib.h>
#include <string.h>

#include <cesanta/frozen.h>


/***********************************************************************************************
 * SCOPED STRUCTS DECLARATIONS
 **********************************************************************************************/

/**
 * @brief Wrapped configuration.
 */
typedef struct wrap_configuration_s {
    p_client_configuration client_configuration; // Client configuration.
    p_server_configuration server_configuration; // Server configuration.

    struct configuration_s * (*add_client_cfg)(struct configuration_s *cfg); // Add VK API configuration.
    struct configuration_s * (*add_server_cfg)(struct configuration_s *cfg); // Add server configuration.

    char *raw_json_data;    // Raw data from configuration file.
    int raw_json_data_size; // Raw data size.
} wrap_configuration_t, *p_wrap_configuration;


/***********************************************************************************************
 * STATIC FUNCTIONS DECLARATIONS
 **********************************************************************************************/

/**
 * @brief Read raw JSON data from file.
 *
 * @param wrapped_cfg Wrapped configuration object.
 * @param path Path to configuration file.
 */
static void read_raw_json_data(p_wrap_configuration wrapped_cfg, const char *path);

/**
 * @brief Add VK API configuration.
 *
 * @param cfg Configuration object.
 * @return Configuration object.
 */
static p_configuration add_client_cfg(p_configuration cfg);

/**
 * @brief Add server configuration.
 *
 * @param cfg Configuration object.
 * @return Configuration object.
 */
static p_configuration add_server_cfg(p_configuration cfg);

/**
 * @brief Initialize VK API configuration.
 *
 * @param wrapped_cfg Wrapped configuration object.
 * @return VK API configuration object.
 */
static p_client_configuration init_client_cfg(p_wrap_configuration wrapped_cfg);

/**
 * @brief Initialize server configuration.
 *
 * @param wrapped_cfg Wrapped configuration object.
 * @return Server configuration object.
 */
static p_server_configuration init_server_cfg(p_wrap_configuration wrapped_cfg);

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

p_configuration init_configuration(const char *path) {
    p_wrap_configuration wrapped_cfg = (p_wrap_configuration)malloc(sizeof(wrap_configuration_t));
    p_configuration cfg = (p_configuration)wrapped_cfg;
    if (!wrapped_cfg)
        return NULL;

    read_raw_json_data(wrapped_cfg, path);

    cfg->client_configuration = NULL;
    cfg->server_configuration = NULL;
    cfg->add_client_cfg = add_client_cfg;
    cfg->add_server_cfg = add_server_cfg;
    
    return cfg;
}

void release_configuration(p_configuration cfg) {
    p_wrap_configuration wrapped_cfg = (p_wrap_configuration)cfg;
    if (cfg) {
        if (cfg->client_configuration)
            release_client_cfg(wrapped_cfg);
        if (cfg->server_configuration)
            release_server_cfg(wrapped_cfg);
        if (wrapped_cfg->raw_json_data)
            free(wrapped_cfg->raw_json_data);
        free(wrapped_cfg);
    }
}


/***********************************************************************************************
 * STATIC FUNCTIONS DEFINITIONS
 **********************************************************************************************/

static void read_raw_json_data(p_wrap_configuration wrapped_cfg, const char *path) {
    wrapped_cfg->raw_json_data = json_fread(path);
    wrapped_cfg->raw_json_data_size = strlen(wrapped_cfg->raw_json_data);
}

static p_configuration add_client_cfg(p_configuration cfg) {
    cfg->client_configuration = init_client_cfg((p_wrap_configuration)cfg);
    return cfg;
}

static p_configuration add_server_cfg(p_configuration cfg) {
    cfg->server_configuration = init_server_cfg((p_wrap_configuration)cfg);
    return cfg;
}

static p_client_configuration init_client_cfg(p_wrap_configuration wrapped_cfg) {
    p_client_configuration client_cfg = (p_client_configuration)malloc(sizeof(client_configuration_t));
    if (!client_cfg)
        return NULL;

    client_cfg->vk_api_auth_uri = NULL;
    client_cfg->vk_api_cert = NULL;

    const char *path;
    json_scanf(wrapped_cfg->raw_json_data, wrapped_cfg->raw_json_data_size, "{CLIENT_SETTINGS:%Q}", &path);

    const char *json_vk_data = json_fread(path);
    const int json_vk_data_size = strlen(json_vk_data);
    json_scanf(json_vk_data, json_vk_data_size, "{VK_API:{AUTHORIZE_URI:%Q}}", &client_cfg->vk_api_auth_uri);
    json_scanf(json_vk_data, json_vk_data_size, "{VK_API:{ACCESS_CERTIFICATE:%Q}}", &client_cfg->vk_api_cert);
    
    return client_cfg;
}

static p_server_configuration init_server_cfg(p_wrap_configuration wrapped_cfg) {
    p_server_configuration server_cfg = (p_server_configuration)malloc(sizeof(server_configuration_t));
    if (!server_cfg)
        return NULL;

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
    p_configuration cfg = (p_configuration)wrapped_cfg;
    if (cfg) {
        p_client_configuration client_cfg = (p_client_configuration)cfg->client_configuration;
        if (client_cfg) {
            if (client_cfg->vk_api_auth_uri)
                free(client_cfg->vk_api_auth_uri);
            if (client_cfg->vk_api_cert)
                free(client_cfg->vk_api_cert);
            free(client_cfg);
        }
    }
}

static void release_server_cfg(p_wrap_configuration wrapped_cfg) {
    p_configuration cfg = (p_configuration)wrapped_cfg;
    if (cfg) {
        p_server_configuration server_cfg = (p_server_configuration)cfg->server_configuration;
        if (server_cfg) {
            if (server_cfg->host)
                free(server_cfg->host);
            if (server_cfg->cors_policy)
                free(server_cfg->cors_policy);
            free(server_cfg);
        }
    }
}
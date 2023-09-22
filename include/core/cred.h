/**
 * @file cred.h
 * @brief Credential information.
 * @date 2023-09-09
 */

#ifndef EEPARS_CREDS_H
#define EEPARS_CREDS_H

/**
 * @brief Credentials.
 */
typedef struct {
    const char *json; // JSON data.
    int size; // JSON text length.

    const char *app_settigns; // Application settings path;
    const char *auth_vk_api_uri; // Authorize VK API URI.
    const char *vk_api_cert; // VK API certificate.
} credentials_t, *p_credentials;

/**
 * @brief Register data.
 */
extern p_credentials register_credentials(const char *filename);

/**
 * @brief Release data.
 */
extern void release_credentials(p_credentials this);

#endif // EEPARS_CREDS_H
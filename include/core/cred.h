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
    const char *json; // JSON credentials.
    int size; // JSON text length.

    const char *auth_vk_api_uri; // Authorize VK API URI.
    const char *bff_uri; // BFF host URI.
    const char *frontend_uri; // Frontend host URI.

    const char *vk_api_cert; // VK API certificate.
    const char *app_settigns; // Application settings path;
} credentials_t, *p_credentials;

/**
 * @brief Register credentials.
 */
extern p_credentials register_credentials(const char *filename);

/**
 * @brief Release credentials.
 */
extern void release_credentials(p_credentials this);

#endif // EEPARS_CREDS_H
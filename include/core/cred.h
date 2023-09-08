#ifndef EEPARS_CREDS_H
#define EEPARS_CREDS_H

/**
 * @brief Credentials.
 */
typedef struct {
    const char *json; // JSON credentials.
    const char *uri; // Host URI.
} credentials_t, *p_credentials;

/**
 * @brief Register credentials.
 */
extern p_credentials register_credentials(const char *path_to_credentials);

/**
 * @brief Release credentials.
 */
extern void release_credentials(p_credentials credentials);

#endif // EEPARS_CREDS_H
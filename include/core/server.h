/**
 * @file server.h
 * @brief BFF server.
 * @date 2023-09-09
 */

#ifndef EEPARS_SERVER_H
#define EEPARS_SERVER_H

#include <pthread.h>
#include <cesanta/mongoose.h>

typedef struct {
    const char *cors_policy;
} server_settings_t, *p_server_settings;

/**
 * @brief Server structure.
 */
typedef struct {
    pthread_t server_thread;
    p_server_settings server_settings;
    struct mg_mgr mgr;
} server_t, *p_server;

/**
 * @brief Register server.
 */
extern p_server register_server(const char *uri, const char *settings);

/**
 * @brief Release server.
 */
extern void release_server(p_server this);

#endif // EEPARS_SERVER_H
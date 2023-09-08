#ifndef EEPARS_SERVER_H
#define EEPARS_SERVER_H

#include <pthread.h>
#include <cesanta/mongoose.h>

/**
 * @brief Server structure.
 */
typedef struct {
    pthread_t server_thread;
    struct mg_mgr mgr;
} server_t, *p_server;

/**
 * @brief Register server.
 */
p_server register_server(const char *uri);

/**
 * @brief Release server.
 */
void release_server(p_server this);

#endif // EEPARS_SERVER_H
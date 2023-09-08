#ifndef EEPARS_VKCLIENT_H
#define EEPARS_VKCLIENT_H

#include <cesanta/mongoose.h>

/**
 * @brief Client structure.
 */
typedef struct {
    struct mg_mgr mgr;
    struct mg_connection *c;
} client_t, *p_client;

/**
 * @brief Register client.
 * 
 * @return p_client - pointer to registered client.
 */
p_client register_client(void);

/**
 * @brief Release client.
 * 
 * @param this Client to release.
 */
void release_client(p_client this);

#endif //EEPARS_VKCLIENT_H
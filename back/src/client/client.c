#include <client/client.h>

#include <app/service.h>


/*********************************************************************************************
 * STATIC FUNCTIONS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Run client.
 *
 * @param client Client object.
 */
static void client_run(p_client client);


/*********************************************************************************************
 * FUNCTIONS DEFINITIONS
 ********************************************************************************************/

p_client init_client(p_client_configuration client_cfg) {
    p_client client = malloc(sizeof(client_t));

    client->configuration = client_cfg;
    client->run = client_run;
    
    return client;
}

void release_client(p_client client) {
    if (client) {
        mg_mgr_free(&client->manager);
        free(client);
    }
}


/*********************************************************************************************
 * STATIC FUNCTIONS DEFINITIONS
 ********************************************************************************************/

static void client_run(p_client client) {
    mg_log_set(MG_LL_NONE); // Set log level.
    mg_mgr_init(&client->manager); // Initialise event manager.
}
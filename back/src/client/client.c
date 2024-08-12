#include <client/client.h>

#include <container.h>
#include <macro.h>

/*********************************************************************************************
 * STATIC FUNCTIONS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Run client.
 *
 * @param client Client object.
 */
static void run(p_client client);

/*********************************************************************************************
 * FUNCTIONS DEFINITIONS
 ********************************************************************************************/

p_client init_client(void) {
    p_client client = malloc(sizeof(client_t));
    if (!client) return NULL;

    p_configuration configuration = get_service_from_global_container(name_of(p_configuration));
    p_client_configuration client_cfg = configuration->client_configuration;

    client->configuration = client_cfg;
    client->run = run;
    
    return client;
}

void release_client(p_client client) {
    if (!client) return;
        
    mg_mgr_free(&client->manager);
    free(client);
    
    client = NULL;
}

/*********************************************************************************************
 * STATIC FUNCTIONS DEFINITIONS
 ********************************************************************************************/

static void run(p_client client) {
    mg_log_set(MG_LL_NONE); // Set log level.
    mg_mgr_init(&client->manager); // Initialise event manager.
}
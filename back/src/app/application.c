#include <app/application.h>

#include <stdlib.h>

#include <client/client.h>
#include <server/server.h>

#include <container.h>
#include <macro.h>

/*********************************************************************************************
 * STATIC FUNCTIONS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Run application.
 */
static void run(void);

/**
 * @brief Add server.
 *
 * @return Application object.
 */
static p_application add_server(void);

/**
 * @brief Add client.
 *
 * @return Application object.
 */
static p_application add_client(void);

/*********************************************************************************************
 * FUNCTIONS DEFINITIONS
 ********************************************************************************************/

p_application init_application(void) {
    p_application app = (p_application)malloc(sizeof(application_t));
    if (!app) return NULL;

    app->cfg = NULL;
    app->server = NULL;
    app->client = NULL;

    app->run = run;
    app->add_server = add_server;
    app->add_client = add_client;

    return app;
}

void release_application(p_application app) {
    if (!app) return;

    free(app);

    app = NULL;
}

/*********************************************************************************************
 * STATIC FUNCTIONS DEFINITIONS
 ********************************************************************************************/

static p_application add_server(void) {
    p_container container = get_container(name_of(main));
    p_application app = get_service_from_container(container, name_of(p_application));
    if (!app) return NULL;

    app->server = get_service_from_container(container, name_of(p_server));
    return app;
}

static p_application add_client(void) {
    p_container container = get_container(name_of(main));
    p_application app = get_service_from_container(container, name_of(p_application));
    if (!app) return NULL;
    
    app->client = get_service_from_container(container, name_of(p_client));
    return app;
}

static void run(void) {
    p_container container = get_container(name_of(main));
    p_application app = get_service_from_container(container, name_of(p_application));
    if (app) {
        app->server->run(app->server);
        app->client->run(app->client);

        pthread_join(app->server->thread, &(void *) {0});
    }
}
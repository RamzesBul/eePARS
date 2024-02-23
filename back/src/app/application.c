#include <app/application.h>

#include <stdlib.h>

#include <client/client.h>
#include <server/server.h>

#include <container.h>

/*********************************************************************************************
 * STATIC FUNCTIONS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Run application.
 */
static void run();

/**
 * @brief Add server.
 *
 * @param cfg Configuration object.
 * @return Application object.
 */
static p_application add_server(p_configuration cfg);

/**
 * @brief Add client.
 *
 * @param cfg Configuration object.
 * @return Application object.
 */
static p_application add_client(p_configuration cfg);

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
}

/*********************************************************************************************
 * STATIC FUNCTIONS DEFINITIONS
 ********************************************************************************************/

static p_application add_server(p_configuration cfg) {
    p_application app = get_service_from_container(name_of(p_application));
    if (cfg && app) {
        if (!app->cfg) app->cfg = cfg;

        p_server_configuration server_cfg = app->cfg->server_configuration;
        app->server = get_service_from_container(name_of(p_server));
    }
}

static p_application add_client(p_configuration cfg) {
    p_application app = get_service_from_container(name_of(p_application));
    if (cfg && app) {
        if (!app->cfg) app->cfg = cfg;

        p_client_configuration client_cfg = app->cfg->client_configuration;
        app->client = get_service_from_container(name_of(p_client));
    }
}

static void run() {
    p_application app = get_service_from_container(name_of(p_application));
    if (app) {
        app->server->run(app->server);
        app->client->run(app->client);

        pthread_join(app->server->thread, &(void *) {0});
    }
}
#include <app/application.h>

#include <stdlib.h>

#include <client/client.h>
#include <server/server.h>


/*********************************************************************************************
 * SCOPED STRUCTS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Wrapped application.
 */
typedef struct wrap_application_s {
    struct application_s *(*add_server)(struct application_s * app, p_services services); // Add server.
    struct application_s *(*add_client)(struct application_s * app, p_services services); // Add client.
    void (*run)(struct application_s *app); // Run application.

    p_services services; // Services object;
    p_server server;     // Server.
    p_client client;     // Client.
} wrap_application_t, *p_wrap_application;


/*********************************************************************************************
 * STATIC FUNCTIONS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Run application.
 *
 * @param app Application object.
 */
static void run(p_application app);

/**
 * @brief Add server.
 *
 * @param app Application object.
 * @param services Services object.
 * @return Application object.
 */
static p_application add_server(p_application app, p_services services);

/**
 * @brief Add client.
 *
 * @param app Application object.
 * @param services Services object.
 * @return Application object.
 */
static p_application add_client(p_application app, p_services services);


/*********************************************************************************************
 * STATIC VARIABLES
 ********************************************************************************************/
static p_wrap_application instance = NULL;


/*********************************************************************************************
 * FUNCTIONS DEFINITIONS
 ********************************************************************************************/

p_application init_application(void) {
    p_wrap_application wrapped_app = (p_wrap_application)malloc(sizeof(wrap_application_t));
    p_application app = (p_application)wrapped_app;
    if (!wrapped_app)
        return NULL;

    instance = wrapped_app;

    wrapped_app->services = NULL;
    wrapped_app->server = NULL;
    wrapped_app->client = NULL;

    app->run = run;
    app->add_server = add_server;
    app->add_client = add_client;

    return app;
}

p_client get_app_client_instance() {
    return instance->client;
}

p_server get_app_server_instance() {
    return instance->server;
}

void release_application(p_application app) {
    p_wrap_application wrapped_app = (p_wrap_application)app;
    if (app) {
        if (wrapped_app->server)
            release_server(wrapped_app->server); 
        if (wrapped_app->client)
            release_client(wrapped_app->client); 
        free(app);
    }
}


/*********************************************************************************************
 * STATIC FUNCTIONS DEFINITIONS
 ********************************************************************************************/

static p_application add_server(p_application app, p_services services) {
    if (services) {
        p_wrap_application wrapped_app = (p_wrap_application)app;
        if (!wrapped_app->services)
            wrapped_app->services = services;
        
        p_services app_services = wrapped_app->services;
        p_configuration cfg = app_services->configuration;
        if (cfg) {
            p_server_configuration server_cfg = cfg->server_configuration;
            wrapped_app->server = init_server(server_cfg);
        }
    }
}

static p_application add_client(p_application app, p_services services) {
    if (services) {
        p_wrap_application wrapped_app = (p_wrap_application)app;
        if (!wrapped_app->services)
            wrapped_app->services = services;

        p_services app_services = wrapped_app->services;
        p_configuration cfg = app_services->configuration;
        if (cfg) {
            p_client_configuration client_cfg = cfg->client_configuration;
            wrapped_app->client = init_client(client_cfg);
        }
    }
}

static void run(p_application app) {
    if (app) {
        p_server app_server = ((p_wrap_application)app)->server;
        app_server->run(app_server);

        p_client app_client = ((p_wrap_application)app)->client;
        app_client->run(app_client);
    }
}
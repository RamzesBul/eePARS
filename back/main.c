#include <app/configuration.h>
#include <app/application.h>
#include <server/server.h>
#include <client/client.h>
#include <db/postgres.h>
#include <db/db_connection.h>
#include <user/user.h>

#include <threadpool.h>
#include <container.h>
#include <macro.h>

/***********************************************************************************************
* STATIC FUNCTIONS DECLARATIONS
**********************************************************************************************/

/**
* @brief Register services.
* 
* @param container Container.
*/
static void register_services(p_container container);

/***********************************************************************************************
* STATIC VARIABLES
**********************************************************************************************/

static const char *config_file_path = "../data/appsettings.json";

/***********************************************************************************************
* FUNCTIONS DEFINITIONS
**********************************************************************************************/

int main(int argc, char *argv[]) {
    init_thread_pool();
 
    p_container container = init_container(name_of(main));
    register_services(container);
    
    // Set configuration.
    const p_configuration configuration = get_service_from_container(container, name_of(p_configuration));
    configuration->open_cfg_file();
    configuration->add_server_cfg();
    configuration->add_client_cfg();
    
    // Connfiguring application.
    const p_application application = get_service_from_container(container, name_of(p_application));
    application->add_server_api();
    application->add_client_api();
    
    // Running application.
    application->run();
    
    release_container(container);
    
    destroy_thread_pool();
    return 0;
}

/***********************************************************************************************
* STATIC FUNCTIONS DEFINITIONS
**********************************************************************************************/

static void register_services(p_container container) {
    // Globalvalue services.
    add_glblvalue_to_container(container, name_of(CONFIG_FILE_PATH), config_file_path, NULL);
    
    // Singleton services.
    add_singleton_to_container(container, name_of(p_configuration), init_configuration, release_configuration);
    add_singleton_to_container(container, name_of(p_application), init_application, release_application);
    add_singleton_to_container(container, name_of(p_server), init_server, release_server);
    add_singleton_to_container(container, name_of(p_client), init_client, release_client);
    add_singleton_to_container(container, name_of(p_postgres), init_postgres, release_postgres);
    
    // Transient services.
    add_transient_to_container(container, name_of(p_connection), init_connection, release_connection);
    add_transient_to_container(container, name_of(p_user), init_user, release_user);
}
#include <app/application.h>
#include <app/configuration.h>

#include <container.h>
#include <macro.h>

#define CFG_FILE "../data/appsettings.json"

/**
 * @brief Register services.
 */
static void register_services();

int main(int argc, char *argv[]) {
    p_container container = init_container(name_of(main));
    register_services();

    // Set configuration.
    const p_configuration configuration = get_service_from_container(container, name_of(p_configuration));
    configuration->open_cfg_file(CFG_FILE);
    configuration->add_server_cfg();
    configuration->add_client_cfg();
    
    // Connfiguring application.
    const p_application application = get_service_from_container(container, name_of(p_application));
    application->add_server();
    application->add_client();

    // Running application.
    application->run();

    release_container(container);
    return 0;
}

static void register_services() {
    p_container container = get_container(name_of(main));

    add_singleton_to_container(container, name_of(p_configuration), init_configuration, release_configuration);
    add_singleton_to_container(container, name_of(p_application), init_application, release_application);
    add_singleton_to_container(container, name_of(p_server), init_server, release_server);
    add_singleton_to_container(container, name_of(p_client), init_client, release_client);
}
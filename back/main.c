#include <container.h>

#include <app/application.h>
#include <app/configuration.h>

#ifdef DEV
#define CFG_FILE "../data/dev/appsettings.json"
#else
#define CFG_FILE "../data/local/appsettings.json"
#endif

int main() {
    init_container();

    // Setting up configuration service.
    add_singleton_to_container(name_of(p_configuration), init_configuration, release_configuration);
    const p_configuration configuration = get_service_from_container(name_of(p_configuration));
    configuration->open_cfg_file(CFG_FILE);
    configuration->add_server_cfg();
    configuration->add_client_cfg();

    // Setting up application service.
    add_singleton_to_container(name_of(p_application), init_application, release_application);
    add_singleton_to_container(name_of(p_server), init_server, release_server);
    add_singleton_to_container(name_of(p_client), init_client, release_client);
    const p_application application = get_service_from_container(name_of(p_application));
    application->add_server();
    application->add_client();

    // Running application.
    application->run();

    release_container();
    return 0;
}
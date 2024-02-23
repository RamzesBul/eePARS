#include <container.h>

#include <app/configuration.h>
#include <app/application.h>

int main() {
    init_container();

    add_service_to_container(SERVICE_TYPE_SINGLETON, name_of(p_configuration), init_configuration, release_configuration, NULL);
    p_configuration configuration = get_service_from_container(name_of(p_configuration));
    configuration->open_cfg_file("../data/appsettings.json");
    configuration->add_server_cfg();
    configuration->add_client_cfg();

    add_service_to_container(SERVICE_TYPE_SINGLETON, name_of(p_application), init_application, release_application, NULL);
    add_service_to_container(SERVICE_TYPE_SINGLETON, name_of(p_server), init_server, release_server, configuration->server_configuration);
    add_service_to_container(SERVICE_TYPE_SINGLETON, name_of(p_client), init_client, release_client, configuration->client_configuration);
    p_application application = get_service_from_container(name_of(p_application));
    application->add_server(configuration);
    application->add_client(configuration);
    
    application->run();

    release_container();
    return 0;
}
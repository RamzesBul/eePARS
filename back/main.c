#include <core/configuration.h>
#include <app/service.h>
#include <app/application.h>

int main() {
    p_configuration configuration = init_configuration("../data/appsettings.json");
    configuration->add_server_cfg(configuration);
    configuration->add_client_cfg(configuration);

    p_services services = init_services();
    services->add_configuration(services, configuration);

    p_application application = init_application();
    application->add_server(application, services);
    application->add_client(application, services);
    application->run(application);

    release_application(application);
    release_services(services);
    release_configuration(configuration);

    return 0;
}
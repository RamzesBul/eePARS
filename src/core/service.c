#include <core/service.h>

p_services register_services(void) {
    p_services this = malloc(sizeof(services_t));

    this->credentials = register_credentials("../credentials/credentials.json");
    this->server = register_server(this->credentials->uri);

    return this;
}

void release_services(p_services this) {
    release_server(this->server);
    release_credentials(this->credentials);

    free(this);
}
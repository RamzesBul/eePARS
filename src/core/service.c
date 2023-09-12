#include <core/service.h>

p_services register_services(void) {
    p_services this = malloc(sizeof(services_t));

    if (this) {
        this->credentials = register_credentials("../credentials/credentials.json");
        this->server = register_server(this->credentials->bff_uri, this->credentials->app_settigns);

        return this;
    }

    return NULL;
}

void release_services(p_services this) {
    if (this) {
        if (this->server) release_server(this->server);
        if (this->credentials) release_credentials(this->credentials);

        free(this);
    }
}
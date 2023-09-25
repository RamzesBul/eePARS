#include <core/service.h>

p_services services;

int main() {
    services = register_services(); // Register services.
    release_services(services); // Release services.
    return 0;
}
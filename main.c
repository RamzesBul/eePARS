#include <core/service.h>

int main() {
    p_services services = register_services(); // Register services.
    release_services(services); // Release services.
    return 0;
}
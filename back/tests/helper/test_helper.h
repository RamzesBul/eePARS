#ifndef EEPARS_TEST_HELPER_H
#define EEPARS_TEST_HELPER_H

#include <container.h>

#include <app/application.h>
#include <app/configuration.h>

/**
 * @brief Testing environment.
 */
typedef struct env_s {
    p_configuration cfg;
    p_application app;
} env_t, *p_env;

/**
 * @brief Prepare testing environment.
 * 
 * @return Testing environment object.
 */
p_env init_env() {
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
}

/**
 * @brief Release testing environment.
 * 
 * @param env Testing environment object.
 */
void release_env() {
    release_container();
}

/**
 * @brief Compare strings.
 * 
 * @param test_name test name.
 * @param expected expected value.
 * @param actual actual value.
 * @return Comparing result.
 */
int is_equal(const char *test_name, const char *expected, const char *actual) {
    int is_equal = strcmp(expected, actual);

    if (is_equal)
        printf("%s: failed\n", test_name);
    else
        printf("%s: success\n", test_name);
    
    return is_equal;
}

#endif // EEPARS_TEST_HELPER_H
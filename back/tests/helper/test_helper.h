#ifndef EEPARS_TEST_HELPER_H
#define EEPARS_TEST_HELPER_H

#include <app/application.h>
#include <app/service.h>
#include <core/configuration.h>


/**
 * @brief Testing environment.
 */
typedef struct env_s {
    p_configuration cfg;
    p_services services;
    p_application app;
} env_t, *p_env;

/**
 * @brief Prepare testing environment.
 * 
 * @return Testing environment object.
 */
p_env init_env() {
    p_configuration configuration = init_configuration("../data/appsettings.json");
    configuration->add_server_cfg(configuration);
    configuration->add_client_cfg(configuration);

    p_services services = init_services();
    services->add_configuration(services, configuration);

    p_application application = init_application();
    application->add_server(application, services);
    application->add_client(application, services);
    application->run(application);

    p_env env = (p_env)malloc(sizeof(env_t));

    env->cfg = configuration;
    env->services = services;
    env->app = application;

    return env;
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
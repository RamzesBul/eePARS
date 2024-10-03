#ifndef EEPARS_TEST_HELPER_H
#define EEPARS_TEST_HELPER_H

#include <app/application.h>
#include <app/configuration.h>

#include <container.h>
#include <macro.h>

#define CFG_FILE "../data/appsettings.json"

/*********************************************************************************************
 * STRUCTS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Testing environment.
 */
typedef struct env_s {
    p_configuration cfg;
    p_application app;
} env_t, *p_env;

/*********************************************************************************************
 * FUNCTIONS DEFINITIONS
 ********************************************************************************************/

/**
 * @brief Prepare testing environment.
 * 
 * @return Testing environment object.
 */
p_env init_env(void) {
    p_container container = init_container(name_of(main));

    add_singleton_to_container(container, name_of(p_configuration), init_configuration, release_configuration);
    add_singleton_to_container(container, name_of(p_application), init_application, release_application);
    add_singleton_to_container(container, name_of(p_server), init_server, release_server);
    add_singleton_to_container(container, name_of(p_client), init_client, release_client);

    p_configuration configuration = get_service_from_container(container, name_of(p_configuration));
    configuration->open_cfg_file(CFG_FILE);
    configuration->add_server_cfg();
    configuration->add_client_cfg();
    
    p_application application = get_service_from_container(container, name_of(p_application));
    application->add_server();
    application->add_client();
}

/**
 * @brief Release testing environment.
 * 
 * @param env Testing environment object.
 */
void release_env(void) {
    release_container_by_name(name_of(main));
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
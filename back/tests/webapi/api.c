#include <string.h>

#include <app/application.h>
#include <app/configuration.h>

#include <container.h>
#include <macro.h>

#define ORDER_RESULT(result, order_num) (!result) << order_num
#define CFG_FILE "../data/appsettings.json"

/*********************************************************************************************
 * STATIC FUNCTIONS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Prepare testing environment.
 * 
 * @return Testing environment object.
 */
static void init_env(void);

/**
 * @brief Compare strings.
 * 
 * @param expected Expected value.
 * @param actual Actual value.
 * @return Comparing result.
 */
static int is_equal(const char *expected, const char *actual);

/*********************************************************************************************
 * FUNCTIONS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Test for WebAPI.
 *
 * @return Test result.
 */
int test_webapi_OK(void);

/*********************************************************************************************
 * FUNCTIONS DEFINITIONS
 ********************************************************************************************/

int main(void) {
    int exit_result = 0;

    exit_result |= test_webapi_OK();
    
    return exit_result;
}

int test_webapi_OK(void) {
    init_env();

    return 0;
}

/*********************************************************************************************
 * STATIC FUNCTIONS DEFINITIONS
 ********************************************************************************************/

static void init_env(void) {
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

    release_container(container);
}

static int is_equal(const char *expected, const char *actual) {
    return strcmp(expected, actual) == 0;
}
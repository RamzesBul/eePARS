#include <string.h>

#include <app/application.h>
#include <app/configuration.h>
#include <client/client.h>
#include <db/db_connection.h>
#include <db/postgres.h>
#include <server/server.h>

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

/***********************************************************************************************
 * STATIC VARIABLES
 **********************************************************************************************/

static const char *config_file_path = "../data/appsettings.json";

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
    add_glblvalue_to_container(container, name_of(CONFIG_FILE_PATH),
                               config_file_path, NULL);
    add_singleton_to_container(container, name_of(p_configuration),
                               init_configuration, release_configuration);
    add_singleton_to_container(container, name_of(p_application),
                               init_application, release_application);
    add_singleton_to_container(container, name_of(p_server), init_server,
                               release_server);
    add_singleton_to_container(container, name_of(p_client), init_client,
                               release_client);
    add_singleton_to_container(container, name_of(p_postgres), init_postgres,
                               release_postgres);
    add_transient_to_container(container, name_of(p_connection),
                               init_connection, release_connection);

    p_configuration configuration =
        get_service_from_container(container, name_of(p_configuration));
    configuration->open_cfg_file();
    configuration->add_server_cfg();
    configuration->add_client_cfg();

    p_application application =
        get_service_from_container(container, name_of(p_application));
    application->add_server_api();
    application->add_client_api();

    release_container(container);
}

static int is_equal(const char *expected, const char *actual) {
    return strcmp(expected, actual) == 0;
}
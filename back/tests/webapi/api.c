#include <stdio.h>

#include <app/application.h>
#include <app/service.h>
#include <client/api.h>
#include <core/configuration.h>

#include <helper/test_helper.h>

/**
 * @brief Test for WelcomeController.
 *
 * @return Test result.
 */
int test_Welcome_OK();

/**
 * @brief Test for AuthorizationController.
 *
 * @return Test result.
 */
int test_Authorize_OK();

int main() {
    p_env env = init_env();
    int exit_result = 0;

    exit_result |= test_Welcome_OK();
    exit_result |= test_Authorize_OK();

    return exit_result;
}

int test_Welcome_OK() {
    char *expected =
        "https://oauth.vk.com/"
        "authorize?client_id=51737169&display=page&redirect_uri=http://"
        "localhost:4200/"
        "authorization&scope=friends&response_type=token&v=5.131&state=123456";

    p_client client = get_app_client_instance();
    char *actual = request_get(client, "http://localhost:8080/welcome");

    return is_equal("test_Welcome_OK", expected, actual);
}

int test_Authorize_OK() {
    return is_equal("test_Authorize_OK", "", "");
}
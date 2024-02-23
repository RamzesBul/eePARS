#include <stdio.h>

#include <app/application.h>
#include <app/configuration.h>
#include <client/api.h>

#include <container.h>

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
    init_env();
    int exit_result = 0;

    exit_result |= test_Welcome_OK();

    release_env();
    return exit_result;
}

int test_Welcome_OK() {
    char *expected =
        "https://oauth.vk.com/"
        "authorize?client_id=51737169&display=page&redirect_uri=http://"
        "localhost:4200/"
        "authorization&scope=friends&response_type=token&v=5.131&state=123456";

    char *actual = request_get("http://localhost:8080/welcome");

    return is_equal("test_Welcome_OK", expected, actual);
}
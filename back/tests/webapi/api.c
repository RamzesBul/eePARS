#include <stdio.h>

#include <app/application.h>
#include <app/configuration.h>
#include <client/api.h>

#include <container.h>

#include <helper/test_helper.h>

#define ORDER_RESULT(result, order_num) (!result) << order_num

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
    release_env();

    return 0;
}
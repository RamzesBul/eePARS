#include <server/controllers/welcome.h>

#include <db/db_connection.h>
#include <user/user.h>

#include <container.h>
#include <macro.h>

/***********************************************************************************************
 * FUNCTIONS DEFINITIONS
 **********************************************************************************************/

void *welcome(struct mg_http_message *hm) {
    p_container container = get_container(name_of(main));
    p_user user = get_service_from_container_with_args(container, name_of(p_user), hm);

    if (user)
        return user->token;

    return NULL;
}
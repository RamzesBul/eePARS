#include <server/controllers/registration.h>

#include <db/db_connection.h>
#include <server/api.h>
#include <user/user.h>

#include <container.h>
#include <macro.h>

/***********************************************************************************************
 * FUNCTIONS DEFINITIONS
 **********************************************************************************************/

void *registration(struct mg_http_message *hm) {
    p_container container = get_container(name_of(main));
    p_user user = get_service_from_container_with_args(container, name_of(p_user), hm);

    if (user)
        return user->token;

    const char *login = from_query(hm, name_of(login));
    const char *password = from_query(hm, name_of(password));
    const char *email = from_query(hm, name_of(email));

    const char *sql = stringify(
        insert into eepars_user(login, password, email, role)
        values($1, $2, $3, 1);
    );
    execute(sql, 3, login, password, email);

    return NULL;
}
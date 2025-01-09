#include <server/controllers/authorization.h>

#include <db/db_connection.h>
#include <server/api.h>
#include <user/user.h>

#include <container.h>
#include <macro.h>

/***********************************************************************************************
 * FUNCTIONS DEFINITIONS
 **********************************************************************************************/

void *authorization(struct mg_http_message *hm) {
    p_container container = get_container(name_of(main));
    p_user user = get_service_from_container_with_args(container, name_of(p_user), hm);

    if (user)
        return user->token;

    const char *login = from_query(hm, name_of(login));
    const char *password = from_query(hm, name_of(password));

    const char *get_user_sql = stringify(
        select id from eepars_user eu where eu.login =
        $1 and eu.password = $2;
    );
    const char ***get_user_query_result = query(get_user_sql, 2, login, password);

    const char *create_new_token_sql = stringify(
        insert into user_auth_history(user_id, token, expiration_date)
        values($1, extract(epoch from current_timestamp)::varchar, current_timestamp + interval '1 hour');
        returning token;
    );
    const char ***result = query(create_new_token_sql, 1, ***get_user_query_result);

    return ***result;
}
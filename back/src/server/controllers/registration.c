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
    
    const char *login = from_body(hm, name_of(login));
    const char *password = from_body(hm, name_of(password));
    const char *email = from_body(hm, name_of(email));
    
    const char *sql = stringify(
        insert into eepars_user(login, password, email, role)
        values($1, $2, $3, 1)
        on conflict (login) do update 
        set login = eepars_user.login
        returning id;
    );
    p_db_result insert_query_result = query(sql, 3, login, password, email);
    const char *insert_result = release_db_result(insert_query_result, 1, 1);

    const char *create_new_token_sql = stringify(
        insert into user_auth_history(user_id, token, expiration_date)
        values($1::integer, extract(epoch from current_timestamp)::integer::varchar, current_timestamp + interval '1 hour')
        returning token;
    );
    p_db_result token_query_result = query(create_new_token_sql, 1, insert_result);
    free(insert_result);

    return release_db_result(token_query_result, 1, 1);
}
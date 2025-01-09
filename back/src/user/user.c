#include <user/user.h>

#include <stdlib.h>

#include <db/db_connection.h>
#include <server/api.h>

#include <container.h>
#include <macro.h>

/***********************************************************************************************
 * FUNCTIONS DEFINITIONS
 **********************************************************************************************/

p_user init_user(struct mg_http_message *hm) {
    p_user user = (p_user)malloc(sizeof(user_t));
    if (!user)
        return NULL;

    p_container container = get_container(name_of(main));
    p_connection connection = get_service_from_container(container, name_of(p_connection));

    user->token = from_header(hm, name_of(token));
    const char *sql = "                                                                 \
        select eu.id,                                                                   \
               eu.login,                                                                \
               eu.email,                                                                \
               eu.role                                                                  \
        from eepars_user eu                                                             \
          inner join user_auth_history uah on eu.id = uah.user_id                       \
                                          and uah.expiration_date >= current_timestamp  \
        where uah.token = $1;                                                           \
    ";
    char ***result = query(sql, 1, user->token);
    if (result && result[0]) {
        user->id = atoi(result[0][0]);
        user->login = result[0][1];
        user->password = NULL;
        user->email = result[0][2];
        user->role = atoi(result[0][3]);
    } else {
        return NULL;
    }

    return user;
}

void release_user(p_user user) {
    if (!user || (user && user->id <= 0))
        return;

    if (user->login)
        free(user->login);
    if (user->token)
        free(user->token);
    if (user->email)
        free(user->email);

    free(user);
}
#include <server/controllers/user.h>

#include <cesanta/frozen.h>

#include <server/api.h>
#include <user/user.h>

#include <container.h>
#include <macro.h>

/***********************************************************************************************
* FUNCTIONS DEFINITIONS
**********************************************************************************************/

void *getUserInfo(struct mg_http_message *hm) {
    p_container container = get_container(name_of(main));
    p_user user = get_service_from_container_with_args(container, name_of(p_user), hm);
    
    if (user) {
        const int len = 512;
        const char *result = malloc(len);

        struct json_out out = JSON_OUT_BUF(result, len);
        json_printf(&out, "{id: %d, login: %Q, email: %Q, role: %d}",
            user->id,
            user->login,
            user->email,
            user->role
        );
        return result;
    }
    
    return NULL;
}
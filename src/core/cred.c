#include <core/cred.h>
#include <cesanta/frozen.h>
#include <stdlib.h>
#include <string.h>

p_credentials register_credentials(const char *path_to_credentials) {
    p_credentials this = malloc(sizeof(credentials_t));

    if (this) {
        this->json = json_fread(path_to_credentials);
        this->size = strlen(this->json);

        json_scanf(this->json, this->size, "{BFF_HOST:%Q}", &this->bff_uri);
        json_scanf(this->json, this->size, "{FRONTEND_HOST:%Q}", &this->frontend_uri);
        json_scanf(this->json, this->size, "{VK_API_AUTHORIZE_URI:%Q}", &this->auth_vk_api_uri);
        json_scanf(this->json, this->size, "{VK_API_CERT:%Q}", &this->vk_api_cert);
        json_scanf(this->json, this->size, "{APP_SETTINGS:%Q}", &this->app_settigns);

        return this;
    }

    return NULL;
}

void release_credentials(p_credentials this) {
    if (this) {
        if (this->bff_uri) free(this->bff_uri);
        if (this->frontend_uri) free(this->bff_uri);
        if (this->auth_vk_api_uri) free(this->auth_vk_api_uri);
        if (this->vk_api_cert) free(this->bff_uri);
        if (this->app_settigns) free(this->bff_uri);

        if (this->json) free(this->json);
        free(this);
    }
}
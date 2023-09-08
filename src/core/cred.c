#include <core/cred.h>
#include <cesanta/frozen.h>
#include <stdlib.h>
#include <string.h>

p_credentials register_credentials(const char *path_to_credentials) {
    p_credentials this = malloc(sizeof(credentials_t));

    this->json = json_fread(path_to_credentials);
    json_scanf(this->json, strlen(this->json), "{VK_API_AUTHORIZE_URI:%Q}", &this->uri);

    return this;
}

void release_credentials(p_credentials this) {
    free(this->uri);
    free(this->json);
    free(this);
}
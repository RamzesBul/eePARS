#include <db/postgres.h>

#include <stdlib.h>
#include <string.h>

#include <app/configuration.h>

#include <container.h>
#include <macro.h>

/***********************************************************************************************
* STATIC FUNCTIONS DECLARATIONS
**********************************************************************************************/

/**
* @brief Get postgres connection parameters.
* 
* @param postgres Postgres instance.
*/
static void get_db_params(p_postgres postgres);

/**
* @brief Get postgres connection parameter.
* 
* @param postgres Postgres instance.
* @param param Param.
* @param param_name Param name.
*/
static void get_db_param(p_postgres postgres, char **param, const char *param_name);

/***********************************************************************************************
* FUNCTIONS DEFINITIONS
**********************************************************************************************/

p_postgres init_postgres(void) {
    p_container container = get_container(name_of(main));
    p_configuration configuration = get_service_from_container(container, name_of(p_configuration));
    
    p_postgres postgres = (p_postgres)malloc(sizeof(postgres_t));
    if (!postgres) return NULL;
    
    postgres->conn = configuration->server_configuration->postrges_db_connection;
    get_db_params(postgres);
    
    return postgres;
}

void release_postgres(p_postgres postgres) {
    if (!postgres) return;
    
    if (postgres->conn) free(postgres->conn);
    if (postgres->db_name) free(postgres->db_name);
    if (postgres->user) free(postgres->user);
    if (postgres->password) free(postgres->password);
    if (postgres->host) free(postgres->host);
    if (postgres->port) free(postgres->port);
    if (postgres->options) free(postgres->options);

    free(postgres);
    postgres = NULL;
}

/***********************************************************************************************
* STATIC FUNCTIONS DEFINITIONS
**********************************************************************************************/

static void get_db_params(p_postgres postgres) {
    if (!postgres) return;
    
    get_db_param(postgres, &postgres->user, "user=");
    get_db_param(postgres, &postgres->password, "password=");
    get_db_param(postgres, &postgres->db_name, "dbname=");
    get_db_param(postgres, &postgres->host, "host=");
    get_db_param(postgres, &postgres->port, "port=");
    get_db_param(postgres, &postgres->options, "options=");
}

static void get_db_param(p_postgres postgres, char **param,  const char *param_name) {
    int param_name_length = strlen(param_name);
    char *param_substr = strstr(postgres->conn, param_name);
    if (param_substr) {
        char *end = strstr(param_substr, " ");
        if (!end) {
            end = (long long)param_substr + strlen(param_substr);
        }
        if (end) {
            int length = (long long)end - ((long long)param_substr + param_name_length) + 1;
            if (length > 0) {
                *param = (char*)malloc(length);
                strncpy(*param, param_substr + param_name_length, length);
                (*param)[length - 1] = '\0';
            }
        }
    }
}
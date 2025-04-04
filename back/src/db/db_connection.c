#include <db/db_connection.h>

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <libpq-fe.h>

#include <container.h>
#include <macro.h>


/***********************************************************************************************
* STATIC FUNCTIONS DECLARATIONS
**********************************************************************************************/

/**
* @brief Database data clear function.
* 
* @param connection PostgreSQL connection.
*/
static void connection_release(p_connection connection);

/***********************************************************************************************
* FUNCTIONS DEFINITIONS
**********************************************************************************************/

p_connection init_connection(void) {
    p_container container = get_container(name_of(main));
    p_postgres postgres = get_service_from_container(container, name_of(p_postgres));
    
    p_connection connection = (p_connection)malloc(sizeof(connection_t));
    if (!connection)
        return NULL;
    
    connection->postgres = postgres;
    connection->conn = PQconnectdb(postgres->conn);
    connection->result = NULL;
    
    if (PQstatus(connection->conn) == CONNECTION_BAD) {
        PQfinish(connection->conn);
        exit(1);
    }
    
    if (strcmp(connection->postgres->user, PQuser(connection->conn)) ||
        strcmp(connection->postgres->password, PQpass(connection->conn)) ||
        strcmp(connection->postgres->db_name, PQdb(connection->conn))) {
            PQfinish(connection->conn);
    }
    
    return connection;
}

void release_connection(p_connection connection) {
    if (!connection)
        return;

    connection_release(connection);

    free(connection);
}

p_db_result execute(const char *query, int args_count, ...) {
    p_container container = get_container(name_of(main));
    p_connection connection = get_service_from_container(container, name_of(p_connection));
    
    if (!connection)
        return NULL;
    
    if (args_count) {
        va_list args;
        va_start(args, args_count);
        
        const char **params = (char **)malloc(args_count * sizeof(char *));
        for (int i = 0; i < args_count; i++) {
            const char *param = va_arg(args, const char *);
            params[i] = param;
        }
        connection->result = PQexecParams(connection->conn, query, args_count, NULL, params, NULL, NULL, 0);
        
        va_end(args);
    } else {
        connection->result = PQexec(connection->conn, query);
    }
    
    p_db_result db_result = (p_db_result)malloc(sizeof(db_result_t));
    db_result->data = NULL;
    db_result->error = NULL;
    db_result->rows_count = 0;
    db_result->columns_count = 0;
    
    if (PQresultStatus(connection->result) != PGRES_TUPLES_OK) {
        const char *error = PQresultErrorMessage(connection->result);
        int length = strlen(error);
        if (error && length) {
            db_result->error = malloc(length * sizeof(char) + 1);
            memcpy(db_result->error, error, length + 1);
        } else {
            memcpy(db_result->error, "", 1);
        }
    }

    connection_release(connection);
    
    return db_result;
}

p_db_result query(const char *query, int args_count, ...) {
    p_container container = get_container(name_of(main));
    p_connection connection = get_service_from_container(container, name_of(p_connection));
    if (!connection)
        return NULL;
    
    if (args_count) {
        va_list args;
        va_start(args, args_count);
        
        const char **params = (char **)malloc(args_count * sizeof(char *));
        for (int i = 0; i < args_count; i++) {
            const char *param = va_arg(args, const char *);
            params[i] = param;
        }
        connection->result = PQexecParams(connection->conn, query, args_count, NULL, params, NULL, NULL, 0);
        free(params);
        
        va_end(args);
    } else {
        connection->result = PQexec(connection->conn, query);
    }
    
    p_db_result db_result = (p_db_result)malloc(sizeof(db_result_t));
    db_result->data = NULL;
    db_result->error = NULL;
    db_result->rows_count = 0;
    db_result->columns_count = 0;
    
    if (PQresultStatus(connection->result) == PGRES_TUPLES_OK) {
        db_result->rows_count = PQntuples(connection->result);
        db_result->columns_count = PQnfields(connection->result);
        if (db_result->rows_count > 0 && db_result->columns_count > 0) {
            db_result->data = (char ***)malloc(db_result->rows_count * sizeof(char **));
            for (int i = 0; i < db_result->rows_count; i++) {
                db_result->data[i] = (char **)malloc(db_result->columns_count * sizeof(char *));
                for (int j = 0; j < db_result->columns_count; j++) {
                    const char *value = PQgetvalue(connection->result, i, j);
                    int length = PQgetlength(connection->result, i, j);
                    db_result->data[i][j] = (char *)malloc(length * sizeof(char) + 1);
                    if (value && length) {
                        memcpy(db_result->data[i][j], value, length + 1);
                    } else {
                        memcpy(db_result->data[i][j], "", 1);
                    }
                }
            }
        }
    } else {
        const char *error = PQresultErrorMessage(connection->result);
        int length = strlen(error);
        if (error && length) {
            db_result->error = malloc(length * sizeof(char) + 1);
            memcpy(db_result->error, error, length + 1);
        } else {
            memcpy(db_result->error, "", 1);
        }
    }

    connection_release(connection);
    
    return db_result;
}

void *release_db_result(p_db_result result, int row_id, int col_id) {
    if (!result)
    return NULL;
    
    if (result->error) {
        free(result->error);
        result->error = NULL;
    }
    
    char **data = NULL;
    if (result->data) {
        if (row_id == -1) {
            data = result->data;
        } else {
            for (int i = 0; i < result->rows_count; i++) {
                if (row_id == i + 1 && (col_id == 0 || col_id == -1)) {
                    data = result->data[i];
                    continue;
                }
                for (int j = 0; j < result->columns_count; j++) {
                    if (row_id == i + 1 && col_id == j + 1) {
                        data = result->data[i][j];
                        continue;
                    }
                    free(result->data[i][j]);
                    result->data[i][j] = NULL;
                }
                free(result->data[i]);
                result->data[i] = NULL;
            }
            free(result->data);
            result->data = NULL;
        }
    }
    free(result);
    
    return data;
}


/***********************************************************************************************
* STATIC FUNCTIONS DEFINITIONS
**********************************************************************************************/

static void connection_release(p_connection connection) {
    if (!connection)
        return;

    if (connection->result) {
        PQclear(connection->result);
        connection->result = NULL;
    }
    if (connection->conn) {
        PQfinish(connection->conn);
        connection->conn = NULL;
    }
    connection->postgres = NULL;
}
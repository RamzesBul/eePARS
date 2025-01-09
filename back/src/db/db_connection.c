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
* @param conn PostgreSQL connection.
* @param res PostgreSQL result.
*/
static void db_clear(PGconn *conn, PGresult *res);

/***********************************************************************************************
* FUNCTIONS DEFINITIONS
**********************************************************************************************/

p_connection init_connection(void) {
    p_container container = get_container(name_of(main));
    p_postgres postgres = get_service_from_container(container, name_of(p_postgres));
    
    p_connection connection = (p_connection)malloc(sizeof(connection_t));
    if (!connection) return NULL;
    
    connection->postgres = postgres;
    connection->conn = PQconnectdb(postgres->conn);
    connection->result = NULL;
    
    if (PQstatus(connection->conn) == CONNECTION_BAD) {
        db_clear(connection->conn, NULL);
        exit(1);
    }
    
    if (strcmp(connection->postgres->user, PQuser(connection->conn)) ||
    strcmp(connection->postgres->password, PQpass(connection->conn)) ||
    strcmp(connection->postgres->db_name, PQdb(connection->conn)))
        db_clear(connection->conn, NULL);
    
    return connection;
}

void release_connection(p_connection connection) {
    if (!connection) return;
    
    db_clear(connection->conn, connection->result);
    
    free(connection);
    connection = NULL;
}

void execute(const char *query, int args_count, ...) {
    p_container container = get_container(name_of(main));
    p_connection connection = get_service_from_container(container, name_of(p_connection));
    if (!connection) return;
    
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
    
    db_clear(connection->conn, connection->result);
}

char ***query(const char *query, int args_count, ...) {
    p_container container = get_container(name_of(main));
    p_connection connection = get_service_from_container(container, name_of(p_connection));
    if (!connection) return NULL;
    
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
    
    const char ***table = NULL;
    if (PQresultStatus(connection->result) == PGRES_TUPLES_OK) {
        int row_count = PQntuples(connection->result);
        int column_count = PQnfields(connection->result);
        if (row_count > 0 && column_count > 0) {
            table = (char ***)malloc(row_count * sizeof(char **));
            for (int i = 0; i < row_count; i++) {
                table[i] = (char **)malloc(column_count * sizeof(char *));
                for (int j = 0; j < column_count; j++)
                table[i][j] = PQgetvalue(connection->result, i, j);
            }
        }
    }
    
    return table;
}

/***********************************************************************************************
* STATIC FUNCTIONS DEFINITIONS
**********************************************************************************************/

static void db_clear(PGconn *conn, PGresult *res) {
    if (res) PQclear(res);
    if (conn) PQfinish(conn);    
}
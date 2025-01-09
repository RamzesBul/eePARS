#ifndef EEPARS_DB_CONNECTION_H
#define EEPARS_DB_CONNECTION_H

#include <stdarg.h>

#include <libpq-fe.h>

#include <db/postgres.h>

/*********************************************************************************************
 * STRUCTS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Connection to postgres database.
 */
typedef struct connection_s {
    PGconn *conn;        // Connection.
    PGresult *result;    // Result.
    p_postgres postgres; // Postgres data.
} connection_t, *p_connection;

/*********************************************************************************************
 * FUNCTIONS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Initialize connection to database.
 *
 * @return Connection;
 */
extern p_connection init_connection(void);

/**
 * @brief Release connection to database.
 *
 * @param connection Connection.
 */
extern void release_connection(p_connection connection);

/**
 * @brief Execute query to database.
 *
 * @details
 * Do not return result.
 *
 * @param query Query.
 * @param args_count Arguments count.
 * @param args Arguments.
 */
extern void execute(const char *query, int args_count, ...);

/**
 * @brief Query to database.
 *
 * @details
 * Return result.
 *
 * @param query Query.
 * @param args_count Arguments count.
 * @param args Arguments.
 *
 * @return Result table.
 */
extern char ***query(const char *query, int args_count, ...);

#endif // EEPARS_DB_CONNECTION_H
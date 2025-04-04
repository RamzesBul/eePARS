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

/**
* @brief Query result.
*/
typedef struct db_result_s {
    const char *error;       // Error message.
    const char ***data;      // Result table.
    int rows_count;    // Rows count.
    int columns_count; // Columns count.
} db_result_t, *p_db_result;

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
extern p_db_result execute(const char *query, int args_count, ...);

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
extern p_db_result query(const char *query, int args_count, ...);

/**
 * @brief Release db result.
 * 
 * @param result Query result.
 * @param row_id Row id to return (with 1 to rows_count).
 * @param col_id Column id to return (with 1 to columns_count).
 * @return Return cell value if row_id and col_id is valid.
 *         Return row if row_id is valid and col_id is -1 or 0.
 *         Return all rows if row_id is -1.
 *         Release all rows if row_id is 0.
 */
extern void *release_db_result(p_db_result result, int row_id, int col_id);

#endif // EEPARS_DB_CONNECTION_H
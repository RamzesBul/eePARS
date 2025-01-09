#ifndef EEPARS_POSTGRES_H
#define EEPARS_POSTGRES_H

/*********************************************************************************************
 * STRUCTS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief PostgreSQL database data.
 */
typedef struct postgres_s {
    char *conn;     // Connection string.
    char *db_name;  // Database name.
    char *user;     // User name.
    char *password; // Password.
    char *host;     // Host name.
    char *port;     // Port number.
    char *options;  // Options.
} postgres_t, *p_postgres;

/*********************************************************************************************
 * FUNCTIONS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Initialize postrges.
 *
 * @return Postgres;
 */
extern p_postgres init_postgres(void);

/**
 * @brief Release postgres.
 *
 * @param postgres Postgres.
 */
extern void release_postgres(p_postgres postgres);

#endif // EEPARS_POSTGRES_H
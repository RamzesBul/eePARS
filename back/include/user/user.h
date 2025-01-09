#ifndef EEPARS_USER_H
#define EEPARS_USER_H

#include <cesanta/mongoose.h>

/*********************************************************************************************
 * STRUCTS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief User role.
 */
typedef enum user_role_e {
    READER = 0,     // Reader.
    WRITER = 1,     // Writer.
    SUPERVISOR = 2, // Supervisor.
} user_role_t, *p_user_role;

/**
 * @brief User object.
 */
typedef struct user_s {
    long id;          // User identifier.
    char *login;      // User login.
    char *password;   // User password.
    char *email;      // User email.
    char *token;      // User token.
    user_role_t role; // User role.
} user_t, *p_user;

/*********************************************************************************************
 * FUNCTIONS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Initialize user.
 *
 * @param hm HTTP message.
 * @return User.
 */
extern p_user init_user(struct mg_http_message *hm);

/**
 * @brief Release user.
 *
 * @param user User.
 */
extern void release_user(p_user user);

#endif // EEPARS_USER_H
#ifndef EEPARS_CLIENT_API_H
#define EEPARS_CLIENT_API_H

/*******************************************************************************
 * FUNCTION DECLARATIONS
 ******************************************************************************/

/**
 * @brief Send GET request.
 *
 * @param url - URL.
 * @return Response.
 */
extern char *request_get(const char *url);

#endif // EEPARS_CLIENT_API_H
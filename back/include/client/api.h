#ifndef EEPARS_CLIENT_API_H
#define EEPARS_CLIENT_API_H

#include <client/client.h>


/*******************************************************************************
 * FUNCTION DECLARATIONS
 ******************************************************************************/

/**
 * @brief Send GET request.
 *
 * @param client Client object.
 * @param url - URL.
 * @return Response.
 */
extern char *request_get(p_client client, const char *url);

#endif // EEPARS_CLIENT_API_H
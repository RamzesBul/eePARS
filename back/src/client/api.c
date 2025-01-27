#include <client/api.h>

#include <client/client.h>

#include <container.h>
#include <macro.h>

/*******************************************************************************
 * STRUCTS DECLARATION
 ******************************************************************************/

/**
 * @brief Function data.
 * 
 * @details
 * This struct is used to get the response from the request.
 */
typedef struct fn_data_s {
    const char *url; // Target URL.
    int done;        // Response flag.
    char *response;  // Response data.
    p_client client; // Client object.
} fn_data_t, *p_fn_data;

/*********************************************************************************************
 * STATIC FUNCTIONS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Callback function for client.
 *
 * @param c Connection.
 * @param ev Event.
 * @param ev_data Event data.
 * @param fn_data Function data.
 */
static void request_callback(struct mg_connection *c, int ev, void *ev_data, void *fn_data);

/*******************************************************************************
 * FUNCTIONS DEFINITIONS
 ******************************************************************************/

char *request_get(const char *url) {
    p_container container = get_container(name_of(main));
    p_client client = get_service_from_container(container, name_of(p_client));

    fn_data_t fn_data = { .url = url, .done = 0, .response = NULL, .client = client };

    client->connection = mg_http_connect(&client->manager, url, request_callback, &fn_data);
    while (client->connection && fn_data.done == 0)
        mg_mgr_poll(&client->manager, 1000);

    return fn_data.response;
}

/*********************************************************************************************
 * STATIC FUNCTIONS DEFINITIONS
 ********************************************************************************************/

static void request_callback(struct mg_connection *connection, int ev, void *ev_data, void *fn_data) {
    p_fn_data data = (p_fn_data)fn_data;
    
    if (ev == MG_EV_CONNECT) {
        struct mg_str host = mg_url_host(data->url);
        if (mg_url_is_ssl(data->url)) {
            struct mg_tls_opts opts = { .ca = data->client->configuration->vk_cfg->cert, .name = host };
            mg_tls_init(connection, &opts);
        }
        mg_printf(connection,
                  "GET %s HTTP/1.0\r\n"
                  "Host: %.*s\r\n"
                  "\r\n",
                  mg_url_uri(data->url), (int)host.len, host.buf);
    } else if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *)ev_data;
        data->response = malloc((int)hm->body.len + 1);
        strncpy(data->response, hm->body.buf, (int)hm->body.len);
        data->response[hm->body.len] = 0;
        connection->is_closing = 1;
        data->done = 1;
    } else if (ev == MG_EV_ERROR) {
        data->done = 1;
    }
}
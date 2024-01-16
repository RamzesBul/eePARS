#include <client/api.h>


/*******************************************************************************
 * STRUCTS DECLARATION
 ******************************************************************************/

typedef struct fn_data_s {
    const char *url; // Target URL.
    bool done;       // Response flag.
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

char *request_get(p_client client, const char *url) {
    fn_data_t fn_data = {.url = url, .done = false, .response = NULL, .client = client};

    client->connection = mg_http_connect(&client->manager, url, request_callback, &fn_data);
    while (client->connection && fn_data.done == false)
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
            struct mg_tls_opts opts = {.ca = data->client->configuration->vk_api_cert, .srvname = host};
            mg_tls_init(connection, &opts);
        }
        mg_printf(connection,
                  "GET %s HTTP/1.0\r\n"
                  "Host: %.*s\r\n"
                  "\r\n",
                  mg_url_uri(data->url), (int)host.len, host.ptr);
    } else if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *)ev_data;
        data->response = malloc((int)hm->body.len + 1);
        strncpy(data->response, hm->body.ptr, (int)hm->body.len);
        data->response[hm->body.len] = 0;
        connection->is_closing = 1;
        data->done = true;
    } else if (ev == MG_EV_ERROR) {
        data->done = true;
    }
}
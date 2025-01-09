#include <server/api.h>

#include <server/server.h>

#include <stdlib.h>
#include <string.h>

#include <cesanta/frozen.h>

#include <container.h>
#include <macro.h>
#include <threadpool.h>

/***********************************************************************************************
 * STATIC FUNCTIONS DECLARATIONS
 **********************************************************************************************/

/**
 * @brief Execute controller callback.
 * @param invoke_data array of: [0] - controller callback, [1] - http message
 */
static void *execute_controller(void **invoke_data);

/**
 * @brief Parse path and return path.
 *
 * @param path Path to param.
 * @return Procceeded path.
 */
static char *parse_reg_path(const char *path);

/**
 * @brief Make copy of http message.
 *
 * @param hm Http message.
 * @return Copy of http message.
 */
static struct mg_http_message *copy_http_message(struct mg_http_message *hm);

/**
 * @brief Free http message.
 *
 * @details Used for freeing copy http message.ABC
 *
 * @param hm Http message.
 */
static void free_http_message(struct mg_http_message *hm);

/**
 * @brief Callback for on_complete.
 *
 * @param result Result.
 * @param invoke_data Args.
 */
static void on_complete_cb(void *result, void **invoke_data);

/***********************************************************************************************
 * FUNCTIONS DEFINITION
 **********************************************************************************************/

int invoke_controller(const char *url, struct mg_connection *c,
                      struct mg_http_message *hm,
                      void *(*controller)(struct mg_http_message *hm)) {
    if (!hm) return 0;

    if (mg_match(hm->uri, mg_str(url), NULL)) {
        void **invoke_data = malloc(sizeof(3 * sizeof(void *)));
        invoke_data[0] = controller;
        invoke_data[1] = copy_http_message(hm);
        invoke_data[2] = c;

        p_task task = make_task(execute_controller, invoke_data);
        on_complete(task, on_complete_cb, invoke_data);
        
        run_task(as_immediate(task));

        return 1;
    }
    return 0;
}

char *from_query(struct mg_http_message *hm, const char *name) {
    char *param_name = malloc(strlen(name) + 2);
    sprintf(param_name, "%s=", name);
    const int param_name_length = strlen(param_name);

    const char *param_value_start = strstr(hm->query.buf, param_name);
    const char *param_value_end_next = strstr(param_value_start, "&");
    const char *param_value_end = hm->query.buf + hm->query.len;

    if (param_value_start && (param_value_end_next || param_value_end) &&
        param_value_end != param_value_end_next) {
        if (param_value_end_next)
            param_value_end = param_value_end_next;

        const int param_value_length =
            param_value_end - (param_value_start + param_name_length);
        char *param_value = (char *)malloc(param_value_length + 1);
        sprintf(param_value, "%.*s", param_value_length,
                param_value_start + param_name_length);
        param_value[param_value_length] = '\0';

        return param_value;
    }

    return NULL;
}

char *from_body(struct mg_http_message *hm, const char *reg_path) {
    char *path = parse_reg_path(reg_path);

    char *res = NULL;
    if (hm && hm->body.buf)
        json_scanf(hm->body.buf, hm->body.len, path, &res);
    if (path)
        free(path);

    return res;
}

char *from_header(struct mg_http_message *hm, const char *name) {
    struct mg_http_header *headers = hm->headers;
    for (int i = 0; i < MG_MAX_HTTP_HEADERS; i++)
        if (strcmp(headers[i].name.buf, name) == 0)
            return headers[i].value.buf;
    return NULL;
}

/***********************************************************************************************
 * STATIC FUNCTIONS DEFINITION
 **********************************************************************************************/

static void *execute_controller(void **invoke_data) {
    if (!invoke_data)
        return NULL;

    void *(*controller)(struct mg_http_message *hm) = invoke_data[0];
    struct mg_http_message *hm = invoke_data[1];
    struct mg_connection *c = invoke_data[2];

    return controller(hm);

    if (hm)
        free_http_message(hm);
}

static char *parse_reg_path(const char *path) {
    char *end_symbol = path + strlen(path);

    int size = 100;
    char *res = malloc(size);

    int res_pos = 0, url_pos = 0, url_pos_offset = 1;
    res[res_pos] = '{';
    res_pos++;

    char *next_bracket = path;
    char *prev_bracket = path;
    int is_end_symbol = 1;
    int level = 1;
    do {
        if (prev_bracket == next_bracket)
            continue;

        int length = (next_bracket ? next_bracket : end_symbol) - prev_bracket;
        if ((res_pos + length) * 2 >= size) {
            size *= 2;
            res = realloc(res, size);
        }

        memcpy(res + res_pos, path + url_pos, length);

        url_pos += length + url_pos_offset;
        res_pos += length;

        res[res_pos++] = ':';
        if (next_bracket) {
            res[res_pos++] = '{';
            prev_bracket = next_bracket + url_pos_offset;
            level++;
        } else {
            res[res_pos++] = '%';
            res[res_pos++] = 'Q';
            while (level--)
                res[res_pos++] = '}';
            res = realloc(res, res_pos + 1);
            res[res_pos] = '\0';
        }
    } while ((next_bracket = strstr(prev_bracket, "->")) || is_end_symbol--);

    return res;
}

static struct mg_http_message *copy_http_message(struct mg_http_message *hm) {
    struct mg_http_message *res = malloc(sizeof(struct mg_http_message));

    res->body.buf = malloc(hm->body.len);
    res->body.len = hm->body.len;
    memcpy(res->body.buf, hm->body.buf, hm->body.len);

    res->head.buf = malloc(hm->head.len);
    res->head.len = hm->head.len;
    memcpy(res->head.buf, hm->head.buf, hm->head.len);

    for (int i = 0; i < MG_MAX_HTTP_HEADERS; i++) {
        res->headers[i].name.buf = malloc(hm->headers[i].name.len);
        res->headers[i].name.len = hm->headers[i].name.len;
        memcpy(res->headers[i].name.buf, hm->headers[i].name.buf,
               hm->headers[i].name.len);

        res->headers[i].value.buf = malloc(hm->headers[i].value.len);
        res->headers[i].value.len = hm->headers[i].value.len;
        memcpy(res->headers[i].value.buf, hm->headers[i].value.buf,
               hm->headers[i].value.len);
    }

    res->message.buf = malloc(hm->message.len);
    res->message.len = hm->message.len;
    memcpy(res->message.buf, hm->message.buf, hm->message.len);

    res->method.buf = malloc(hm->method.len);
    res->method.len = hm->method.len;
    memcpy(res->method.buf, hm->method.buf, hm->method.len);

    res->proto.buf = malloc(hm->proto.len);
    res->proto.len = hm->proto.len;
    memcpy(res->proto.buf, hm->proto.buf, hm->proto.len);

    res->query.buf = malloc(hm->query.len);
    res->query.len = hm->query.len;
    memcpy(res->query.buf, hm->query.buf, hm->query.len);

    res->uri.buf = malloc(hm->uri.len);
    res->uri.len = hm->uri.len;
    memcpy(res->uri.buf, hm->uri.buf, hm->uri.len);

    return res;
}

static void free_http_message(struct mg_http_message *hm) {
    if (!hm)
        return;

    if (hm->body.buf)
        free(hm->body.buf);
    if (hm->head.buf)
        free(hm->head.buf);
    for (int i = 0; i < MG_MAX_HTTP_HEADERS; i++) {
        if (hm->headers[i].name.buf)
            free(hm->headers[i].name.buf);
        if (hm->headers[i].value.buf)
            free(hm->headers[i].value.buf);
    }
    if (hm->message.buf)
        free(hm->message.buf);
    if (hm->method.buf)
        free(hm->method.buf);
    if (hm->proto.buf)
        free(hm->proto.buf);
    if (hm->query.buf)
        free(hm->query.buf);
    if (hm->uri.buf)
        free(hm->uri.buf);

    free(hm);
}

static void on_complete_cb(void *result, void **invoke_data) {
    if (!result) {
        const char *default_result = "Empty response";
        result = malloc(strlen(default_result) + 1);
        sprintf(result, "%s", default_result);
    }

    p_container container = get_container(name_of(main));
    p_server server = get_service_from_container(container, name_of(p_server));
    struct mg_connection *c = (struct mg_connection *)invoke_data[2];
    mg_http_reply(c, 200, server->configuration->cors_policy, "%s", result);

    free(result);
}
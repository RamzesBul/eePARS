#include <internal_projects/container.h>

#include <stdlib.h>
#include <pthread.h>

#include <internal_projects/dictionary.h>
#include <internal_projects/macro.h>

/***********************************************************************************************
 * STRUCTS DECLARATIONS
 **********************************************************************************************/

/**
 * @brief Application container.
 */
typedef struct application_container_s {
    p_dictionary elements_types;            // Elements types.
    p_dictionary elements_initial_callback; // Elements initial callback.
    p_dictionary elements_release_callback; // Elements release callback.
    p_dictionary elements_args;             // Elements args.
    p_dictionary elements_refs;             // Counts of refs to elements.
    pthread_mutex_t mutex;                  // Mutex.
} application_container_t, *p_application_container;

/***********************************************************************************************
 * STATIC FUNCTIONS DECLARATIONS
 **********************************************************************************************/

/**
 * @brief Release service.
 * 
 * @param key Service key identifier.
 * @param callback Release callback function. 
 */
static void release_service(char *key, container_callback_function callback);

/*********************************************************************************************
 * STATIC VARIABLES
 ********************************************************************************************/

/**
 * @brief Application container instance.
 * 
 * @details
 * Do not for directly access.
 */
static p_application_container container = NULL;

/**
 * @brief Maximum transient references.
 * 
 * @details
 * Maximum number of transient references.
 */
static const int MAX_TRANSIENT_REFS = 2;

/*********************************************************************************************
 * FUNCTIONS DEFINITIONS
 ********************************************************************************************/

void init_container() {
    if (container) return;

    container = (p_application_container)malloc(sizeof(application_container_t));
    container->elements_types = create_dictionary();
    container->elements_initial_callback = create_dictionary();
    container->elements_release_callback = create_dictionary();
    container->elements_args = create_dictionary();
    container->elements_refs = create_dictionary();
    pthread_mutex_init(&container->mutex, NULL);
}

void release_container() {
    if (!container) return;

    pthread_mutex_lock(&container->mutex);
    iterate_over_dictionary(container->elements_release_callback, (void *)release_service);
    delete_dictionary(container->elements_types);
    delete_dictionary(container->elements_initial_callback);
    delete_dictionary(container->elements_release_callback);
    delete_dictionary(container->elements_args);
    delete_dictionary(container->elements_refs);
    pthread_mutex_unlock(&container->mutex);
    pthread_mutex_destroy(&container->mutex);
    free(container);
    container = NULL;
}

void add_glblvalue_to_container(char *key, void *value, container_callback_function release_callback) {
    add_service_to_container(SERVICE_TYPE_GLBLVALUE, key, NULL, release_callback, value);
}

void add_singleton_to_container(char *key, container_callback_function initial_callback, container_callback_function release_callback) {
    add_service_to_container(SERVICE_TYPE_SINGLETON, key, initial_callback, release_callback, NULL);
}

void add_transient_to_container(char *key, container_callback_function initial_callback, container_callback_function release_callback) {
    add_service_to_container(SERVICE_TYPE_TRANSIENT, key, initial_callback, release_callback, NULL);
}

void add_service_to_container(const service_type_t type, char *key,
                              container_callback_function initial_callback,
                              container_callback_function release_callback,
                              void *args) {
    pthread_mutex_lock(&container->mutex);

    container_callback_function callback = get_value_from_dictionary(container->elements_release_callback, key);
    if (callback)
        release_service(key, callback);

    switch (type) {
    case SERVICE_TYPE_SINGLETON:
        add_record_to_dictionary(container->elements_types, key, (void *)SERVICE_TYPE_SINGLETON);
        add_record_to_dictionary(container->elements_initial_callback, key, initial_callback(args));
        add_record_to_dictionary(container->elements_release_callback, key, release_callback);
        add_record_to_dictionary(container->elements_args, key, args);
        break;

    case SERVICE_TYPE_TRANSIENT:
        add_record_to_dictionary(container->elements_types, key, (void *)SERVICE_TYPE_TRANSIENT);
        add_record_to_dictionary(container->elements_initial_callback, key, initial_callback);
        add_record_to_dictionary(container->elements_release_callback, key, release_callback);
        add_record_to_dictionary(container->elements_refs, key, create_dictionary());
        add_record_to_dictionary(container->elements_args, key, args);
        break;

    case SERVICE_TYPE_GLBLVALUE:
        add_record_to_dictionary(container->elements_types, key, (void *)SERVICE_TYPE_GLBLVALUE);
        add_record_to_dictionary(container->elements_initial_callback, key, NULL);
        add_record_to_dictionary(container->elements_release_callback, key, release_callback);
        add_record_to_dictionary(container->elements_args, key, args);
        break;

    default:
        exit(-1);
        break;
    }

    pthread_mutex_unlock(&container->mutex);
}

void *get_service_from_container(char *key) {
    service_type_t type = (service_type_t)get_value_from_dictionary(container->elements_types, key);
    switch (type) {
    case SERVICE_TYPE_SINGLETON:
        return get_value_from_dictionary(container->elements_initial_callback, key);

    case SERVICE_TYPE_TRANSIENT:
        void *args = get_value_from_dictionary(container->elements_args, key);
        p_dictionary refs = get_value_from_dictionary(container->elements_refs, key);
        if (refs->size >= MAX_TRANSIENT_REFS) {
            pthread_mutex_lock(&container->mutex);

            void *head_value = get_head_value_from_dictionary(refs);
            container_callback_function release_callback = get_value_from_dictionary(container->elements_release_callback, key);
            release_callback(head_value);
            remove_record_from_dictionary_by_index(refs, 0);

            pthread_mutex_unlock(&container->mutex);
        }

        container_callback_function initial_callback = get_value_from_dictionary(container->elements_initial_callback, key);
        add_record_to_dictionary(refs, key, initial_callback(args));
        return get_tail_value_from_dictionary(refs);

    case SERVICE_TYPE_GLBLVALUE:
        return get_value_from_dictionary(container->elements_args, key);

    default:
        break;
    }
}

/***********************************************************************************************
 * STATIC FUNCTIONS DEFINITIONS
 **********************************************************************************************/

static void release_service(char *key, container_callback_function callback) {
    service_type_t type = (service_type_t)get_value_from_dictionary(container->elements_types, key);
    switch (type) {
    case SERVICE_TYPE_SINGLETON:
        callback(get_value_from_dictionary(container->elements_initial_callback, key));
        break;

    case SERVICE_TYPE_TRANSIENT:
        p_dictionary refs = get_value_from_dictionary(container->elements_refs, key);
        container_callback_function release_callback = get_value_from_dictionary(container->elements_release_callback, key);
        iterate_over_dictionary_values(refs, (void *)release_callback);
        delete_dictionary(refs);
        break;

    case SERVICE_TYPE_GLBLVALUE:
        callback(get_value_from_dictionary(container->elements_args, key));
        break;

    default:
        break;
    }
}
#include <internal_projects/container.h>

#include <stdarg.h>
#include <stdlib.h>
#include <mem.h>

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

/*********************************************************************************************
 * FUNCTIONS DEFINITIONS
 ********************************************************************************************/

void init_container() {
    container = (p_application_container)malloc(sizeof(application_container_t));
    container->elements_types = create_dictionary();
    container->elements_initial_callback = create_dictionary();
    container->elements_release_callback = create_dictionary();
    container->elements_args = create_dictionary();
    container->elements_refs = create_dictionary();
}

void release_container() {
    if (!container) return;
    iterate_over_dictionary(container->elements_release_callback, (void *)release_service);
    delete_dictionary(container->elements_types);
    delete_dictionary(container->elements_initial_callback);
    delete_dictionary(container->elements_release_callback);
    delete_dictionary(container->elements_args);
    delete_dictionary(container->elements_refs);
    free(container);
}

void add_service_to_container(const service_type_t type, char *key,
                              container_callback_function initial_callback,
                              container_callback_function release_callback,
                              void *args) {
    if (type == SERVICE_TYPE_SINGLETON) {
        add_record_to_dictionary(container->elements_types, key, (void *)SERVICE_TYPE_SINGLETON);
        add_record_to_dictionary(container->elements_initial_callback, key, initial_callback(args));
        add_record_to_dictionary(container->elements_release_callback, key, release_callback);
        add_record_to_dictionary(container->elements_args, key, args);
    } else if (type == SERVICE_TYPE_TRANSIENT) {
        add_record_to_dictionary(container->elements_types, key, (void *)SERVICE_TYPE_TRANSIENT);
        add_record_to_dictionary(container->elements_initial_callback, key, initial_callback);
        add_record_to_dictionary(container->elements_release_callback, key, release_callback);
        add_record_to_dictionary(container->elements_args, key, args);
        add_record_to_dictionary(container->elements_refs, key, create_dictionary);
    }
}

void *get_service_from_container(char *key) {
    service_type_t type = (service_type_t)get_value_from_dictionary(container->elements_types, key);
    if (type == SERVICE_TYPE_SINGLETON) {
        return get_value_from_dictionary(container->elements_initial_callback, key);
    } else if (type == SERVICE_TYPE_TRANSIENT) {
        void *args = get_value_from_dictionary(container->elements_args, key);
        p_dictionary refs = get_value_from_dictionary(container->elements_refs, key);
        add_record_to_dictionary(refs, key, ((container_callback_function)get_value_from_dictionary(container->elements_initial_callback, key))(args));
        return get_tail_value_from_dictionary(refs);
    }
}

/***********************************************************************************************
 * STATIC FUNCTIONS DEFINITIONS
 **********************************************************************************************/

static void release_service(char *key, container_callback_function callback) {
    service_type_t type = (service_type_t)get_value_from_dictionary(container->elements_types, key);
    if (type == SERVICE_TYPE_SINGLETON) {
        callback(get_value_from_dictionary(container->elements_initial_callback, key));
    } else if (type == SERVICE_TYPE_TRANSIENT) {
        p_dictionary refs = get_value_from_dictionary(container->elements_refs, key);
        container_callback_function release_callback = get_value_from_dictionary(container->elements_release_callback, key);
        iterate_over_dictionary_values(refs, (void *)release_callback);
        delete_dictionary(refs);
    }
}
#include <internal_projects/dictionary.h>

#include <stdlib.h>
#include <string.h>

/***********************************************************************************************
 * FUNCTIONS DEFINITIONS
 **********************************************************************************************/

p_dictionary create_dictionary(void) {
    p_dictionary dict = (p_dictionary)malloc(sizeof(dictionary_t));
    dict->size = 0;
    dict->head = NULL;
    dict->tail = NULL;
    return dict;
}

void delete_dictionary(p_dictionary dict) {
    if (!dict) return;

    p_record current = dict->head;
    while (current) {
        p_record next = current->next;
        free(current);
        current = next;
    }
    free(dict);
    dict = NULL;
}

void add_record_to_dictionary(const p_dictionary dict, char *key, void *value) {
    p_record record = (p_record)malloc(sizeof(record_t));
    record->key = key;
    record->value = value;
    record->next = NULL;
    record->prev = dict->tail;

    if (!dict->size) {
        dict->head = record;
        dict->tail = record;
    } else {
        dict->tail->next = record;
        dict->tail = record;
    }
    dict->size++;
}

void add_record_to_dictionary_by_index(const p_dictionary dict, int index,  char *key, void *value) {
    p_record record = (p_record)malloc(sizeof(record_t));
    record->key = key;
    record->value = value;
    record->next = NULL;
    record->prev = NULL;

    p_record current = get_record_from_dictionary_by_index(dict, index);
    if (current) {
        if (dict->head == current) {
            record->next = current;
            current->prev = record;
            dict->head = record;
        } else {
            record->next = current;
            record->prev = current->prev;
            if (current)
                current->prev = record;
            if (current->prev)
                current->prev->next = record;
        }
        dict->size++;
    }
}

void remove_record_from_dictionary(const p_dictionary dict, char *key) {
    p_record record = dict->head;
    p_record prev = NULL;
    while (record) {
        if (strcmp(record->key, key) == 0) {
            if (prev) {
                prev->next = record->next;
                if (record->next)
                    record->next->prev = prev;
            } else {
                dict->head = record->next;
                if (record->next)
                    record->next->prev = NULL;
            }
            free(record);
            record = NULL;
            dict->size--;
            return;
        }
        prev = record;
        record = record->next;
    }
}

void remove_record_from_dictionary_by_index(const p_dictionary dict, int index) {
    p_record record = dict->head;
    p_record prev = NULL;
    for (int i = 0; i < index; i++) {
        if (!record)
            return;
        prev = record;
        record = record->next;
    }
    if (prev) {
        prev->next = record->next;
        if (record->next)
            record->next->prev = prev;
    } else {
        dict->head = record->next;
        if (record->next)
            record->next->prev = NULL;
    }
    free(record);
    record = NULL;
    dict->size--;
}

void update_record_in_dictionary(const p_dictionary dict, char *key, void *value) {
    p_record record = get_record_from_dictionary(dict, key);
    if (record)
        record->value = value;
}

void update_record_in_dictionary_by_index(const p_dictionary dict, int index, void *value) {
    p_record record = get_record_from_dictionary_by_index(dict, index);
    if (record)
        record->value = value;
}

int contains_key_in_dictionary(const p_dictionary dict, char *key) {
    return get_record_from_dictionary(dict, key) != NULL;
}

int contains_value_in_dictionary(const p_dictionary dict, void *value) {
    return get_record_from_dictionary_by_value(dict, value) != NULL;
}

void *get_value_from_dictionary(const p_dictionary dict, char *key) {
    p_record record = get_record_from_dictionary(dict, key);
    return record ? record->value : NULL;
}

void *get_head_value_from_dictionary(const p_dictionary dict) {
    if (dict->head)
        return dict->head->value;
    return NULL;
}

void *get_tail_value_from_dictionary(const p_dictionary dict) {
    if (dict->tail)
        return dict->tail->value;
    return NULL;
}

void *get_next_value_from_dictionary(const p_dictionary dict, char *key) {
    p_record record = get_record_from_dictionary(dict, key);
    return record && record->next ? record->next->value : NULL;
}

void *get_prev_value_from_dictionary(const p_dictionary dict, char *key) {
    p_record record = get_record_from_dictionary(dict, key);
    return record && record->prev ? record->prev->value : NULL;
}

void *get_head_key_from_dictionary(const p_dictionary dict) {
    if (dict->head)
        return dict->head->key;
    return NULL;
}

void *get_tail_key_from_dictionary(const p_dictionary dict) {
    if (dict->tail)
        return dict->tail->key;
    return NULL;
}

void *get_next_key_from_dictionary(const p_dictionary dict, char *key) {
    p_record record = get_record_from_dictionary(dict, key);
    return record && record->next ? record->next->key : NULL;
}

void *get_prev_key_from_dictionary(const p_dictionary dict, char *key) {
    p_record record = get_record_from_dictionary(dict, key);
    return record && record->prev ? record->prev->key : NULL;
}

p_record get_record_from_dictionary(const p_dictionary dict, char *key) {
    p_record record = dict->head;
    while (record) {
        if (strcmp(record->key, key) == 0)
            return record;
        record = record->next;
    }
    return NULL;
}

p_dictionary get_records_from_dictionary(const p_dictionary dict, char *key) {
    p_dictionary records_dict = create_dictionary();
    p_record record = dict->head;
    while (record) {
        if (strcmp(record->key, key) == 0)
            add_record_to_dictionary(records_dict, record->key, record->value);
        record = record->next;
    }
    return records_dict;
}

p_record get_record_from_dictionary_by_value(const p_dictionary dict, void *value) {
    p_record record = dict->head;
    while (record) {
        if (record->value == value)
            return record;
        record = record->next;
    }
    return NULL;
}

p_dictionary get_records_from_dictionary_by_value(const p_dictionary dict, void *value) {
    p_dictionary records_dict = create_dictionary();
    p_record record = dict->head;
    while (record) {
        if (record->value == value)
            add_record_to_dictionary(records_dict, record->key, record->value);
        record = record->next;
    }
    return records_dict;
}

p_record get_head_record_from_dictionary(const p_dictionary dict) {
    return dict->head;
}

p_record get_tail_record_from_dictionary(const p_dictionary dict) {
    return dict->tail;
}

p_record get_record_from_dictionary_by_index(const p_dictionary dict, int index) {
    p_record record = dict->head;
    for (int i = 0; i < index; i++) {
        if (!record)
            return NULL;
        record = record->next;
    }
    return record;
}

int get_index_from_dictionary_by_key(const p_dictionary dict, char *key) {
    p_record record = dict->head;
    int index = 0;
    while (record) {
        if (strcmp(record->key, key) == 0)
            return index;
        record = record->next;
        index++;
    }
    return -1;
}

int get_index_from_dictionary_by_ref_record(const p_dictionary dict, p_record record) {
    p_record current = dict->head;
    int index = 0;
    while (current) {
        if (current == record)
            return index;
        current = current->next;
        index++;
    }
    return -1;
}

void iterate_over_dictionary(const p_dictionary dict, dictionary_iteration_callback callback) {
    p_record current = dict->head;
    while (current) {
        callback(current->key, current->value);
        current = current->next;
    }
}

void iterate_over_dictionary_with_args(const p_dictionary dict, dictionary_iteration_callback_with_args callback, void *args) {
    p_record current = dict->head;
    while (current) {
        callback(current->key, current->value, args);
        current = current->next;
    }
}

void iterate_over_dictionary_keys(const p_dictionary dict, dictionary_iteration_keys_callback callback) {
    p_record current = dict->head;
    while (current) {
        callback(current->key);
        current = current->next;
    }
}

void iterate_over_dictionary_keys_with_args(const p_dictionary dict, dictionary_iteration_keys_callback_with_args callback, void *args) {
    p_record current = dict->head;
    while (current) {
        callback(current->key, args);
        current = current->next;
    }
}

void iterate_over_dictionary_values(const p_dictionary dict, dictionary_iteration_values_callback callback) {
    p_record current = dict->head;
    while (current) {
        callback(current->value);
        current = current->next;
    }
}

void iterate_over_dictionary_values_with_args(const p_dictionary dict, dictionary_iteration_values_callback_with_args callback, void *args) {
    p_record current = dict->head;
    while (current) {
        callback(current->value, args);
        current = current->next;
    }
}

void iterate_over_dictionary_records(const p_dictionary dict, dictionary_iteration_records_callback callback) {
    p_record current = dict->head;
    while (current) {
        callback(current);
        current = current->next;
    }
}

void iterate_over_dictionary_records_with_args(const p_dictionary dict, dictionary_iteration_records_callback_with_args callback, void *args) {
    p_record current = dict->head;
    while (current) {
        callback(current, args);
        current = current->next;
    }
}

p_dictionary map_dictionary(const p_dictionary dict, dictionary_iteration_callback_map callback) {
    p_dictionary new_dict = create_dictionary();
    p_record current = dict->head;
    while (current) {
        int index = get_index_from_dictionary_by_ref_record(dict, current);
        p_record new_record = callback(current, index, dict);
        add_record_to_dictionary(new_dict, new_record->key, new_record->value);
        current = current->next;
    }
    return new_dict;
}

p_dictionary map_dictionary_with_args(const p_dictionary dict, dictionary_iteration_callback_map_with_args callback, void *args) {
    p_dictionary new_dict = create_dictionary();
    p_record current = dict->head;
    while (current) {
        int index = get_index_from_dictionary_by_ref_record(dict, current);
        p_record new_record = callback(current, index, dict, args);
        add_record_to_dictionary(new_dict, new_record->key, new_record->value);
        current = current->next;
    }
    return new_dict;
}

p_dictionary filter_dictionary(const p_dictionary dict, dictionary_iteration_callback_filter callback) {
    p_dictionary new_dict = create_dictionary();
    p_record current = dict->head;
    while (current) {
        int index = get_index_from_dictionary_by_ref_record(dict, current);
        if (callback(current, index, dict))
            add_record_to_dictionary(new_dict, current->key, current->value);
        current = current->next;
    }
    return new_dict;
}

p_dictionary filter_dictionary_with_args(const p_dictionary dict, dictionary_iteration_callback_filter_with_args callback, void *args) {
    p_dictionary new_dict = create_dictionary();
    p_record current = dict->head;
    while (current) {
        int index = get_index_from_dictionary_by_ref_record(dict, current);
        if (callback(current, index, dict, args))
            add_record_to_dictionary(new_dict, current->key, current->value);
        current = current->next;
    }
    return new_dict;
}

void *reduce_dictionary(const p_dictionary dict, dictionary_iteration_callback_reduce callback, void *acc) {
    p_record current = dict->head;
    while (current) {
        int index = get_index_from_dictionary_by_ref_record(dict, current);
        callback(acc, current, index, dict);
        current = current->next;
    }
    return acc;
}

void *reduce_dictionary_with_args(const p_dictionary dict, dictionary_iteration_callback_reduce_with_args callback, void *acc, void *args) {
    p_record current = dict->head;
    while (current) {
        int index = get_index_from_dictionary_by_ref_record(dict, current);
        callback(acc, current, index, dict, args);
        current = current->next;
    }
    return acc;
}


p_dictionary sort_dictionary(const p_dictionary dict, dictionary_iteration_callback_sort callback) {
    p_dictionary new_dict = create_dictionary();
    p_record current = dict->head;
    while (current) {
        add_record_to_dictionary(new_dict, current->key, current->value);
        current = current->next;
    }
    p_record current1 = new_dict->head;
    while (current1) {
        p_record current2 = current1->next;
        while (current2) {
            if (callback(current1, current2)) {
                char *key = current1->key;
                void *value = current1->value;
                current1->key = current2->key;
                current1->value = current2->value;
                current2->key = key;
                current2->value = value;
            }
            current2 = current2->next;
        }
        current1 = current1->next;
    }
    return new_dict;
}

p_dictionary sort_dictionary_with_args(const p_dictionary dict, dictionary_iteration_callback_sort_with_args callback, void *args) {
    p_dictionary new_dict = create_dictionary();
    p_record current = dict->head;
    while (current) {
        add_record_to_dictionary(new_dict, current->key, current->value);
        current = current->next;
    }
    p_record current1 = new_dict->head;
    while (current1) {
        p_record current2 = current1->next;
        while (current2) {
            if (callback(current1, current2, args)) {
                char *key = current1->key;
                void *value = current1->value;
                current1->key = current2->key;
                current1->value = current2->value;
                current2->key = key;
                current2->value = value;
            }
            current2 = current2->next;
        }
        current1 = current1->next;
    }
    return new_dict;
}
#include <internal_projects/dictionary.h>

#include <stdlib.h>
#include <string.h>

/***********************************************************************************************
 * FUNCTIONS DEFINITIONS
 **********************************************************************************************/

p_dictionary create_dictionary() {
    p_dictionary dict = (p_dictionary)malloc(sizeof(dictionary_t));
    dict->size = 0;
    dict->head = NULL;
    return dict;
}

void delete_dictionary(p_dictionary dict) {
    p_record record = dict->head;
    while (record) {
        p_record next = record->next;
        free(record);
        record = next;
    }
    free(dict);
}

void add_record_to_dictionary(const p_dictionary dict, char *key, void *value) {
    p_record record = (p_record)malloc(sizeof(record_t));
    record->key = key;
    record->value = value;
    record->next = NULL;

    p_record tail = get_tail_record_from_dictionary(dict);
    if (!tail)
        dict->head = record;
    else
        tail->next = record;
    dict->size++;
}

void remove_record_from_dictionary(const p_dictionary dict, char *key) {
    p_record record = dict->head;
    p_record prev = NULL;
    while (record) {
        if (strcmp(record->key, key) == 0) {
            if (prev)
                prev->next = record->next;
            else
                dict->head = record->next;
            free(record);
            dict->size--;
            return;
        }
        prev = record;
        record = record->next;
    }
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
    p_record record = dict->head;
    while (record && record->next)
        record = record->next;
    return record ? record->value : NULL;
}

void *get_next_value_from_dictionary(const p_dictionary dict, char *key) {
    p_record record = get_record_from_dictionary(dict, key);
    return record && record->next ? record->next->value : NULL;
}

void *get_head_key_from_dictionary(const p_dictionary dict) {
    if (dict->head)
        return dict->head->key;
    return NULL;
}

void *get_tail_key_from_dictionary(const p_dictionary dict) {
    p_record record = dict->head;
    while (record && record->next)
        record = record->next;
    return record ? record->key : NULL;
}

void *get_next_key_from_dictionary(const p_dictionary dict, char *key) {
    p_record record = get_record_from_dictionary(dict, key);
    return record && record->next ? record->next->key : NULL;
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

p_record get_head_record_from_dictionary(const p_dictionary dict) {
    return dict->head;
}

p_record get_tail_record_from_dictionary(const p_dictionary dict) {
    p_record record = dict->head;
    while (record && record->next)
        record = record->next;
    return record;
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

void iterate_over_dictionary(const p_dictionary dict, void (*callback)(char *key, void *value)) {
    p_record current = dict->head;
    while (current) {
        callback(current->key, current->value);
        current = current->next;
    }
}

void iterate_over_dictionary_keys(const p_dictionary dict, void (*callback)(char *key)) {
    p_record current = dict->head;
    while (current) {
        callback(current->key);
        current = current->next;
    }
}

void iterate_over_dictionary_values(const p_dictionary dict, void (*callback)(void *value)) {
    p_record current = dict->head;
    while (current) {
        callback(current->value);
        current = current->next;
    }
}

void iterate_over_dictionary_records(const p_dictionary dict, void (*callback)(p_record record)) {
    p_record current = dict->head;
    while (current) {
        callback(current);
        current = current->next;
    }
}
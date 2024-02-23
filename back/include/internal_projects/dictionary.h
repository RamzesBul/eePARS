/*********************************************************************************************
 * @file dictionary.h
 * @author chcp (cmewhou@yandex.ru)
 * @brief Common implementation of an <key:value> pair collection based on a linked list.
 * @version 1.0
 * @date 2024-02-22
 * 
 * @copyright Copyright (c) 2024
 ********************************************************************************************/

#ifndef IPEE_DICTIONARY_H
#define IPEE_DICTIONARY_H

/*********************************************************************************************
 * STRUCTS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Record.
 * 
 * @details
 * Record is a node of linked list.
 * Record is <key:value> pair. It contains a key, value and reference to next node.
 * key - string value.
 * value - pointer to any type value.
 * next - reference to another node.
 * 
 * @warning
 * The implementation is mutable, not thread-safe and not reentrant.
 */
typedef struct record_s {
    struct record_s *next; // Another node reference.
    char *key;             // String key.
    void *value;           // Any type value.
} record_t, *p_record;

/**
 * @brief Dictionary collection.
 * 
 * @details
 * Collection is a linked list of records. Records are not unique.
 * It contains a pointer to actual head record and current size of collection.
 * head - pointer to the head record.
 * size - count of records in collection.
 * 
 * @warning
 * The implementation is mutable, not thread-safe and not reentrant.
 */
typedef struct dictionary_s {
    p_record head; // Head of the dictionary.
    int size;      // Size of the dictionary.
} dictionary_t, *p_dictionary;

/***********************************************************************************************
 * FUNCTION TYPEDEFS
 **********************************************************************************************/

/**
 * @brief Callback function for processing keys and values of collection records.
 * 
 * @param key Record key.
 * @param value Record value.
 */
typedef void (*dictionary_iteration_callback)(char *key, void *value);

/**
 * @brief Callback function for processing keys of collection records.
 * 
 * @param value Record value.
 */
typedef void (*dictionary_iteration_keys_callback)(char *key);

/**
 * @brief Callback function for processing values of collection records.
 * 
 * @param value Record value.
 */
typedef void (*dictionary_iteration_values_callback)(void *value);

/**
 * @brief Callback function for processing records of collection.
 * 
 * @param record Record.
 */
typedef void (*dictionary_iteration_records_callback)(p_record record);

/*********************************************************************************************
 * FUNCTIONS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Create dictionary.
 * 
 * @details
 * Create a new empty dictionary.
 * Fields are NULLs by default.
 *
 * @return Dictionary.
 */
extern p_dictionary create_dictionary();

/**
 * @brief Delete dictionary.
 * 
 * @details
 * Delete dictionary and all its records.
 *
 * @param dict Dictionary object.
 */
extern void delete_dictionary(p_dictionary dict);

/**
 * @brief Add record to dictionary.
 * 
 * @details 
 * Add a new record to specified dictionary.
 * The record is identified by string key and contains value of any type.
 *
 * @param dict Dictionary object.
 * @param key Record key.
 * @param value Record value.
 */
extern void add_record_to_dictionary(const p_dictionary dict, char *key, void *value);

/**
 * @brief Remove record from dictionary.
 * 
 * @details
 * Remove a record from the specified dictionary.
 *
 * @param dict Dictionary object.
 * @param key Record key.
 */
extern void remove_record_from_dictionary(const p_dictionary dict, char *key);

/**
 * @brief Get value from dictionary by key.
 * 
 * @details
 * Get first matching record from specified dictionary.
 * Return the value of found record, if it exists else NULL.
 *
 * @param dict Dictionary.
 * @param key Record key.
 * @return Record value.
 */
extern void *get_value_from_dictionary(const p_dictionary dict, char *key);

/**
 * @brief Get value of head record from dictionary.
 * 
 * @details
 * Get the head record from specified dictionary.
 * Return the value of head record, if it exists else NULL.
 *
 * @param dict Dictionary.
 * @return Record value.
 */
extern void *get_head_value_from_dictionary(const p_dictionary dict);

/**
 * @brief Get value of tail record from dictionary.
 * 
 * @details
 * Get the tail record from specified dictionary.
 * Return the value of tail record, if it exists else NULL.
 *
 * @param dict Dictionary.
 * @return Record value.
 */
extern void *get_tail_value_from_dictionary(const p_dictionary dict);

/**
 * @brief Get value of next record after the current one.
 * 
 * @details
 * Get the next record after the current one from specified dictionary by key of current record.
 * Return the value of next record, if it exists else NULL.
 *
 * @param dict Dictionary.
 * @param key Record key.
 * @return Next record value.
 */
extern void *get_next_value_from_dictionary(const p_dictionary dict, char *key);

/**
 * @brief Get head key from dictionary.
 * 
 * @details
 * Get the head record from specified dictionary.
 * Return the key of head record, if it exists else NULL.
 *
 * @param dict Dictionary.
 * @return Record key.
 */
extern void *get_head_key_from_dictionary(const p_dictionary dict);

/**
 * @brief Get tail key from dictionary.
 * 
 * @details
 * Get the tail record from specified dictionary.
 * Return the key of tail record, if it exists else NULL.
 *
 * @param dict Dictionary.
 * @return Record key.
 */
extern void *get_tail_key_from_dictionary(const p_dictionary dict);

/**
 * @brief Get key of next record after the current one.
 * 
 * @details
 * Get the next record after the current one from specified dictionary by key of current record.
 * Return the key of next record, if it exists else NULL.
 *
 * @param dict Dictionary.
 * @param key Record key.
 * @return Next record key.
 */
extern void *get_next_key_from_dictionary(const p_dictionary dict, char *key);

/**
 * @brief Get record from dictionary.
 * 
 * @details
 * Get first matching record from specified dictionary by key.
 * Return the record, if it exists else NULL.
 *
 * @param dict Dictionary.
 * @param key Record key.
 * @return Record.
 */
extern p_record get_record_from_dictionary(const p_dictionary dict, char *key);

/**
 * @brief Get head record from dictionary.
 * 
 * @details
 * Get the head record from specified dictionary.
 * Return the head record, if it exists else NULL.
 *
 * @param dict Dictionary.
 * @return Record.
 */
extern p_record get_head_record_from_dictionary(const p_dictionary dict);

/**
 * @brief Get tail record from dictionary.
 * 
 * @details
 * Get the tail record from specified dictionary.
 * Return the tail record, if it exists else NULL.
 *
 * @param dict Dictionary.
 * @return Record.
 */
extern p_record get_tail_record_from_dictionary(const p_dictionary dict);

/**
 * @brief Get record from dictionary by index.
 * 
 * @details
 * Get first matching record from specified dictionary by index.
 * Return the record, if it exists else NULL.
 *
 * @param dict Dictionary.
 * @param index Record index.
 * @return Record.
 */
extern p_record get_record_from_dictionary_by_index(const p_dictionary dict, int index);

/**
 * @brief Get index from dictionary by key.
 * 
 * @details
 * Get index of first matching record from specified dictionary by key.
 * Return the index, if it exists else -1.
 *
 * @param dict Dictionary.
 * @param key Record key.
 * @return Record index.
 */
extern int get_index_from_dictionary_by_key(const p_dictionary dict, char *key);

/**
 * @brief Iterate over dictionary.
 * 
 * @details
 * Iterate over all records of specfied dictionary.
 * Apply a callback function to each record.
 * 
 * @param dict Dictionary.
 * @param callback Callback function.
 */
extern void iterate_over_dictionary(const p_dictionary dict, dictionary_iteration_callback callback);

/**
 * @brief Iterate over values of dictionary.
 * 
 * @details
 * Iterate over all values of specfied dictionary.
 * Apply a callback function to each value.
 * 
 * @param dict Dictionary.
 * @param callback Callback function.
 */
extern void iterate_over_dictionary_values(const p_dictionary dict, dictionary_iteration_values_callback callback);

/**
 * @brief Iterate over keys of dictionary.
 * 
 * @details
 * Iterate over all keys of specfied dictionary.
 * Apply a callback function to each key.
 * 
 * @param dict Dictionary.
 * @param callback Callback function.
 */
extern void iterate_over_dictionary_keys(const p_dictionary dict, dictionary_iteration_keys_callback callback);

/**
 * @brief Iterate over dictionary records.
 * 
 * @details
 * Iterate over all records of specfied dictionary.
 * Apply a callback function to each record.
 * 
 * @param dict Dictionary.
 * @param callback Callback function.
 */
extern void iterate_over_dictionary_records(const p_dictionary dict, dictionary_iteration_records_callback callback);

#endif // IPEE_DICTIONARY_H
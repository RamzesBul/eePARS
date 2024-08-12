/**
 * @file event.h
 * @author chcp (cmewhou@yandex.ru)
 * @brief Common event system.
 * @version 1.0
 * @date 2024-07-23
 *
 * @copyright Copyright (c) 2024
 */

#ifndef IPEE_EVENT_H
#define IPEE_EVENT_H

#include <internal_projects/dictionary.h>

/*********************************************************************************************
 * FUNCTIONS DECLARATIONS
 ********************************************************************************************/

/**
 * @brief Subscribe to event globally.
 * 
 * @details
 * Subscribes to event globally.
 * 
 * @param event Event to subscribe to.
 * @param callback Callback function.
 */
extern void global_subscribe(const char *event, void (*callback)(void *args));

/**
 * @brief Subscribe to event.
 * 
 * @details
 * Subscribes to event in the context.
 * 
 * @param context Context to subscribe to.
 * @param event Event to subscribe to.
 * @param callback Callback function.
 */
extern void subscribe(const char *context, const char *event, void (*callback)(void *args));

/**
 * @brief Unsubscribe from event globally.
 * 
 * @details
 * Unsubscribes from event globally.
 * 
 * @param event Event to unsubscribe from.
 */
extern void global_unsubscribe(const char *event, void (*callback)(void *args));

/**
 * @brief Unsubscribe from event.
 * 
 * @details
 * Unsubscribes from event in the context.
 * 
 * @param context Context to unsubscribe from.
 * @param event Event to unsubscribe from.
 */
extern void unsubscribe(const char *context, const char *event, void (*callback)(void *args));

/**
 * @brief Unsubscribe from event globally.
 * 
 * @details
 * Unsubscribes from event globally.
 * 
 * @param event Event to unsubscribe from.
 */
extern void global_notify(const char *event, void *args);

/**
 * @brief Unsubscribe from event.
 * 
 * @details
 * Unsubscribes from event in the context.
 * 
 * @param context Context to unsubscribe from.
 * @param event Event to unsubscribe from.
 */
extern void notify(const char *context, const char *event, void *args);

/**
 * @brief Get the subscriptions.
 * 
 * @details
 * Get subscription list.
 * 
 * @return Subscriptions.
 */
extern p_dictionary get_subscriptions(void);

#endif // IPEE_EVENT_H
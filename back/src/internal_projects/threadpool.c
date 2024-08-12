#include <stdlib.h>

#include <internal_projects/threadpool.h>

/***********************************************************************************************
 * STATIC FUNCTIONS DECLARATIONS
 **********************************************************************************************/

/**
 * @brief Invokes callback of a task in a thread.
 * 
 * @param thread The thread.
 * @return Stub.
 */
static void *task_invoker(p_thread thread);

/**
 * @brief Set the task to thread object.
 * 
 * @param thread The thread.
 * @param task_callback The callback.
 * @param task The task.
 * @param args The arguments.
 */
static void set_task_to_thread(p_thread thread, threadpool_task_callback task_callback, p_task task, void *args);

/*********************************************************************************************
 * STATIC VARIABLES
 ********************************************************************************************/

static const int THREAD_POOL_SIZE = 8;

static thread_t thread_pool[THREAD_POOL_SIZE];

extern void init_thread_pool(void) {
    for (int i = 0; i < THREAD_POOL_SIZE; i++) {
        thread_pool[i].is_buzy = 0;
        thread_pool[i].task_data = NULL;

        pthread_create(&thread_pool[i].thread, NULL, task_invoker, &thread_pool[i]);
    }
}

/*********************************************************************************************
 * FUNCTIONS DEFINITIONS
 ********************************************************************************************/

extern p_task make_task(threadpool_task_callback task_callback, void *args) {
    for (int i = 0; i < THREAD_POOL_SIZE; i++) {
        if (!thread_pool[i].is_buzy) {
            p_task task = (p_task)malloc(sizeof(task_t));
            task->is_done = 0;
            task->result = NULL;

            set_task_to_thread(&thread_pool[i], task_callback, task, args);
            return task;
        }
    }

    return NULL;
}

extern void *await_task(p_task task) {
    while (!task->is_done);

    void *result = task->result;
    free(task);
    return result;
}

extern void destroy_thread_pool(void) {
    for (int i = 0; i < THREAD_POOL_SIZE; i++) {
        pthread_cancel(thread_pool[i].thread);
    }
}

/***********************************************************************************************
 * STATIC FUNCTIONS DEFINITIONS
 **********************************************************************************************/

static void *task_invoker(p_thread thread) {
    while (1) {
        p_task_data task_data = thread->task_data;
        if (task_data->task_callback) {
            void *result = task_data->task_callback(task_data->args);
            task_data->task->is_done = 1;
            task_data->task->result = result;
            free(task_data);

            thread->is_buzy = 0;
            thread->task_data = NULL;
            return result;
        }
    }
}

static void set_task_to_thread(p_thread thread, threadpool_task_callback task_callback, p_task task, void *args) {
    thread->is_buzy= 1;

    p_task_data task_data = (p_task_data)malloc(sizeof(task_data_t));
    task_data->task = task;
    task_data->task_callback = task_callback;
    task_data->args = args;

    thread->task_data = task_data;
}
#ifndef __VDEV_OS_H__
#define __VDEV_OS_H__

#include "vdev_types.h"


typedef void * vdev_os_task_t;
typedef void * vdev_os_mutex_t;
typedef void * vdev_os_event_t;
typedef void * vdev_os_queue_t;

/**
 * @brief Initialize OS
 *
 * @return Status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_os_init_fn) (void);

/**
 * @brief Destroy OS
 *
 * @return Status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_os_destroy_fn) (void);

/**
 * @brief Create task
 *
 * @param[out] task Task handle pointer
 * @param[in] fn Task function
 * @param[in] arg Task function argument
 * @param[in] name Task name
 *
 * @return Status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_os_task_create_fn) (
        _OUT_ vdev_os_task_t *task,
        _IN_ void (*fn)(void *arg),
        _IN_ void *arg,
        _IN_ const char *name);

/**
 * @brief Delete task
 *
 * @param[in] task Task handle
 *
 * @return Status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_os_task_delete_fn) (
        _IN_ vdev_os_task_t task);

/**
 * @brief Start task schedular
 *
 * @return Status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_os_task_start_fn) (void);

/**
 * @brief Create a mutex
 *
 * @param[out] mutex Mutex pointer
 *
 * @return Status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_os_mutex_create_fn) (
        _OUT_ vdev_os_mutex_t *mutex);

/**
 * @brief Delete a mutex
 *
 * @param[in] mutex Mutex
 *
 * @return Status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_os_mutex_delete_fn) (
        _IN_ vdev_os_mutex_t mutex);

/**
 * @brief Lock the mutex
 *
 * @param[in] mutex Mutex
 *
 * @return Status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_os_mutex_lock_fn) (
        _IN_ vdev_os_mutex_t mutex);

/**
 * @brief Unlock the mutex
 *
 * @param[in] mutex Mutex
 *
 * @return Status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_os_mutex_unlock_fn) (
        _IN_ vdev_os_mutex_t mutex);

/**
 * @brief Create a event
 *
 * @param[out] event Event pointer
 *
 * @return Status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_os_event_create_fn) (
        _OUT_ vdev_os_event_t *event);

/**
 * @brief Delete a event
 *
 * @param[in] event Event
 *
 * @return Status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_os_event_delete_fn) (
        _IN_ vdev_os_event_t event);

/**
 * @brief Set event
 *
 * @param[in] event Event
 *
 * @return Status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_os_event_set_fn) (
        _IN_ vdev_os_event_t event);

/**
 * @brief Wait event
 *
 * @param[in] event Event
 *
 * @return Status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_os_event_wait_fn) (
        _IN_ vdev_os_event_t event);

/**
 * @brief Get current task id
 *
 * @return Task id
 */
typedef uint32_t (*vdev_get_task_id_fn) (void);

/**
 * @brief Sleep (second)
 *
 * @param[in] sec Second
 *
 * @return Task id
 */
typedef void (*vdev_sleep_fn) (
        _IN_ uint32_t sec);

/**
 * @brief Sleep (millisecond)
 *
 * @param[in] sec Millisecond
 *
 * @return Task id
 */
typedef void (*vdev_msleep_fn) (
        _IN_ uint32_t ms);

/**
 * @brief Get time (millisecond)
 *
 * @return Task id
 */
typedef uint32_t (*vdev_get_time_fn) (void);

/**
 * @brief Create a queue
 *
 * @param[out] queue Queue pointer
 * @param[in] length Queue length
 * @param[in] itemsize Size of each item
 *
 * @return Status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_os_queue_create_fn) (
        _OUT_ vdev_os_queue_t *queue,
        _IN_ uint32_t length,
        _IN_ uint32_t itemsize);

/**
 * @brief Delete a queue
 *
 * @param[in] queue Queue
 *
 * @return Status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_os_queue_delete_fn) (
        _IN_ vdev_os_queue_t queue);

/**
 * @brief Get a item, the function shall bolck infinitely
 * until put a new item if the queue is empty
 *
 * @param[in] queue Queue
 * @param[out] item item pointer
 *
 * @return Status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_os_queue_get_fn) (
        _IN_ vdev_os_queue_t queue,
        _OUT_ void *item);

/**
 * @brief Put a item into queue, the function shall
 * bolck infinitely until pop a item if the queue
 * is full
 *
 * @param[in] queue Queue
 * @param[in] item item pointer
 *
 * @return Status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_os_queue_put_fn) (
        _IN_ vdev_os_queue_t queue,
        _IN_ void *item);

/**
 * @brief Get a item, it blocks at most ms milliseconds
 * and return VDEV_STATUS_TIMEOUT if queue is empty
 *
 * @param[in] queue Queue
 * @param[out] item item pointer
 *
 * @return Status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_os_queue_get_timeout_fn) (
        _IN_ vdev_os_queue_t queue,
        _OUT_ void *item,
        _IN_ uint32_t ms);

/**
 * @brief Put a item into queue, it blocks at most ms milliseconds
 * and return VDEV_STATUS_TIMEOUT if queue is full
 *
 * @param[in] queue Queue
 * @param[in] item item pointer
 *
 * @return Status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_os_queue_put_timeout_fn) (
        _IN_ vdev_os_queue_t queue,
        _IN_ void *item,
        _IN_ uint32_t ms);

/**
 * @brief OS apis
 */
typedef struct _vdev_os_api_t {
    vdev_os_init_fn          init;
    vdev_os_destroy_fn       destroy;
    /* task */
    vdev_os_task_create_fn   task_create;
    vdev_os_task_delete_fn   task_delete;
    vdev_os_task_start_fn    task_start;
    /* mutex */
    vdev_os_mutex_create_fn  mutex_create;
    vdev_os_mutex_delete_fn  mutex_delete;
    vdev_os_mutex_lock_fn    mutex_lock;
    vdev_os_mutex_unlock_fn  mutex_unlock;
    /* event */
    vdev_os_event_create_fn  event_create;
    vdev_os_event_delete_fn  event_delete;
    vdev_os_event_set_fn     event_set;
    vdev_os_event_wait_fn    event_wait;
    /* misc */
    vdev_get_task_id_fn      get_task_id;
    vdev_sleep_fn            sleep;
    vdev_msleep_fn           msleep;
    vdev_get_time_fn         get_time;
    /* queue */
    vdev_os_queue_create_fn      queue_create;
    vdev_os_queue_delete_fn      queue_delete;
    vdev_os_queue_get_fn         queue_get;
    vdev_os_queue_put_fn         queue_put;
    vdev_os_queue_get_timeout_fn queue_get_timeout;
    vdev_os_queue_put_timeout_fn queue_put_timeout;
} vdev_os_api_t;

#endif

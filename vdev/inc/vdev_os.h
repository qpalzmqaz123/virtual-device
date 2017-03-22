#if !defined(__VDEV_OS_H__) && VDEV_SUPPORT_OS == 1
#define __VDEV_OS_H__

#include "vdev_types.h"

typedef struct _vdev_os_task_t {
    uint32_t id;
} vdev_os_task_t;

typedef void * vdev_os_mutex_t;
typedef void * vdev_os_event_t;

/**
 * @brief Initialize OS
 *
 * @return Status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_os_init_fn) (void);

/**
 * @brief Create task
 *
 * @param[in] name Task name
 * @param[in] fn Task function
 * @param[in] arg Task function argument
 * @param[out] task Task handle
 *
 * @return Status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_os_task_create_fn) (
        _IN_ const char *name,
        _IN_ void (*fn)(void *arg),
        _IN_ void *arg,
        _OUT_ vdev_os_task_t *task);

/**
 * @brief Delete task
 *
 * @param[in] task Task handle
 *
 * @return Status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_os_task_delete_fn) (
        _IN_ vdev_os_task_t *task);

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
typedef vdev_status_t (*vdev_os_unmutex_lock_fn) (
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
 * @param[in] event Event pointer
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
 * @return Task id
 */
typedef void (*vdev_sleep_fn) (uint32_t sec);

/**
 * @brief Sleep (millisecond)
 *
 * @return Task id
 */
typedef void (*vdev_msleep_fn) (uint32_t ms);

/**
 * @brief Get time (millisecond)
 *
 * @return Task id
 */
typedef uint32_t (*vdev_get_time_fn) (void);

/**
 * @brief OS apis
 */
typedef struct _vdev_os_api_t {
    vdev_os_init_fn          init;
    vdev_os_task_create_fn   task_create;
    vdev_os_task_delete_fn   task_delete;
    vdev_os_mutex_create_fn  mutex_create;
    vdev_os_mutex_delete_fn  mutex_delete;
    vdev_os_mutex_lock_fn    mutex_lock;
    vdev_os_unmutex_lock_fn  unmutex_lock;
    vdev_os_event_create_fn  event_create;
    vdev_os_event_delete_fn  event_delete;
    vdev_os_event_set_fn     event_set;
    vdev_os_event_wait_fn    event_wait;
    vdev_get_task_id_fn      get_task_id;
    vdev_sleep_fn            sleep;
    vdev_msleep_fn           msleep;
    vdev_get_time_fn         get_time;
} vdev_os_api_t;

#endif

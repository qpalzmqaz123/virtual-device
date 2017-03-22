#if !defined(__VDEV_OS_H__) && VDEV_SUPPORT_OS == 1
#define __VDEV_OS_H__

#include "vdev_types.h"

typedef struct _vdev_os_task_t {
    uint32_t id;
} vdev_os_task_t;

typedef void * vdev_os_mutex_t;
typedef void * vdev_os_signal_t;

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
typedef vdev_status_t (*vdev_os_create_task_fn) (
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
typedef vdev_status_t (*vdev_os_delete_task_fn) (
        _IN_ vdev_os_task_t *task);

/**
 * @brief Create a mutex
 *
 * @param[out] mutex Mutex pointer
 *
 * @return Status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_os_create_mutex_fn) (
        _OUT_ vdev_os_mutex_t *mutex);

/**
 * @brief Delete a mutex
 *
 * @param[in] mutex Mutex
 *
 * @return Status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_os_delete_mutex_fn) (
        _IN_ vdev_os_mutex_t mutex);

/**
 * @brief Lock the mutex
 *
 * @param[in] mutex Mutex
 *
 * @return Status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_os_lock_mutex_fn) (
        _IN_ vdev_os_mutex_t mutex);

/**
 * @brief Unlock the mutex
 *
 * @param[in] mutex Mutex
 *
 * @return Status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_os_unlock_mutex_fn) (
        _IN_ vdev_os_mutex_t mutex);

/**
 * @brief Create a signal
 *
 * @param[out] signal Signal pointer
 *
 * @return Status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_os_create_signal_fn) (
        _OUT_ vdev_os_signal_t *signal);

/**
 * @brief Delete a signal
 *
 * @param[in] signal Signal pointer
 *
 * @return Status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_os_delete_signal_fn) (
        _IN_ vdev_os_signal_t signal);

/**
 * @brief Set signal
 *
 * @param[in] signal Signal
 *
 * @return Status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_os_set_signal_fn) (
        _IN_ vdev_os_signal_t signal);

/**
 * @brief Wait signal
 *
 * @param[in] signal Signal
 *
 * @return Status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_os_wait_signal_fn) (
        _IN_ vdev_os_signal_t signal);

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
    vdev_os_create_task_fn   create_task;
    vdev_os_delete_task_fn   delete_task;
    vdev_os_create_mutex_fn  create_mutex;
    vdev_os_delete_mutex_fn  delete_mutex;
    vdev_os_lock_mutex_fn    lock_mutex;
    vdev_os_unlock_mutex_fn  unlock_mutex;
    vdev_os_create_signal_fn create_signal;
    vdev_os_delete_signal_fn delete_signal;
    vdev_os_set_signal_fn    set_signal;
    vdev_os_wait_signal_fn   wait_signal;
    vdev_get_task_id_fn      get_task_id;
    vdev_sleep_fn            sleep;
    vdev_msleep_fn           msleep;
    vdev_get_time_fn         get_time;
} vdev_os_api_t;

#endif

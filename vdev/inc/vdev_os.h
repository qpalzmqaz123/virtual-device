#if !defined(__VDEV_OS_H__) && VDEV_SUPPORT_OS == 1
#define __VDEV_OS_H__

#include "vdev_types.h"

typedef struct _vdev_os_task_t {
    uint32_t id;
} vdev_os_task_t;

typedef void * vdev_os_mutex_t;

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
        _IN_ int (*fn)(void *arg),
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
 * @brief OS apis
 */
typedef struct _vdev_os_api_t {
    vdev_os_init_fn         init;
    vdev_os_create_task_fn  create_task;
    vdev_os_delete_task_fn  delete_task;
    vdev_os_create_mutex_fn create_mutex;
    vdev_os_delete_mutex_fn delete_mutex;
    vdev_os_mutex_lock_fn   mutex_lock;
    vdev_os_mutex_unlock_fn mutex_unlock;
} vdev_os_api_t;

#endif

#ifndef __VDEV_BUTTON_H__
#define __VDEV_BUTTON_H__

#include "vdev_types.h"

/**
 * @brief Initialize button
 *
 * @param[in] id Device id
 *
 * @return Return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_button_init_fn) (
        _IN_ uint32_t id);

/**
 * @brief Check if button is pressed down
 *
 * @param[in] id Device id
 *
 * @return Return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_button_is_pressed_fn) (
        _IN_ uint32_t id,
        _OUT_ bool *is_pressed);

/**
 * @brief Listen for press down envent
 *
 * @param[in] id Device id
 * @param[in] cb Callback
 * @param[in] args Callback args
 *
 * @return Return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_button_press_fn) (
        _IN_ uint32_t id,
        _IN_ vdev_async_callback cb,
        _IN_ void *args);

/**
 * @brief Listen for release envent
 *
 * @param[in] id Device id
 * @param[in] cb Callback
 * @param[in] args Callback args
 *
 * @return Return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_button_release_fn) (
        _IN_ uint32_t id,
        _IN_ vdev_async_callback cb,
        _IN_ void *args);

/**
 * @brief button apis
 */
typedef struct _vdev_button_api_t {
    vdev_button_init_fn        init;
    vdev_button_is_pressed_fn  is_pressed;
    vdev_button_press_fn       on_press;
    vdev_button_release_fn     on_release;
} vdev_button_api_t;

#endif

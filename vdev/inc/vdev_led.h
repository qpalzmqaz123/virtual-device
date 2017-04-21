#ifndef __VDEV_LED_H__
#define __VDEV_LED_H__

#include "vdev_types.h"


/**
 * @brief Initialize Led
 *
 * @param[in] id Device id
 *
 * @return Return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_led_init_fn) (
        _IN_ uint32_t id);

/**
 * @brief Turn on led
 *
 * @param[in] id Device id
 *
 * @return Return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_led_on_fn) (
        _IN_ uint32_t id);

/**
 * @brief Turn off led
 *
 * @param[in] id Device id
 *
 * @return Return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_led_off_fn) (
        _IN_ uint32_t id);

/**
 * @brief Toggle led
 *
 * @param[in] id Device id
 *
 * @return Return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_led_toggle_fn) (
        _IN_ uint32_t id);

/**
 * @brief led apis
 */
typedef struct _vdev_led_api_t {
    vdev_led_init_fn      init;
    vdev_led_on_fn        on;
    vdev_led_off_fn       off;
    vdev_led_toggle_fn    toggle;
} vdev_led_api_t;


#endif

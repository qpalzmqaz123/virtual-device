#ifndef __VDEV_FLOWMETER_H__
#define __VDEV_FLOWMETER_H__

#include "vdev_types.h"

/**
 * @brief Initialize flowmeter
 *
 * @param[in] id Device id
 *
 * @return Return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_flowmeter_init_fn) (
        _IN_ uint32_t id);

/**
 * @brief Read flow
 *
 * @param[in] id Device id
 * @param[out] flow Flow (milliliter)
 *
 * @return Return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_flowmeter_read_fn) (
        _IN_ uint32_t id,
        _OUT_ uint32_t *flow);

/**
 * @brief Clear flow
 *
 * @param[in] id Device id
 *
 * @return Return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_flowmeter_clear_fn) (
        _IN_ uint32_t id);

/**
 * @brief Set alarm
 *
 * @param[in] id Device id
 * @param[in] flow Flow
 *
 * @return Return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_flowmeter_set_alarm_fn) (
        _IN_ uint32_t id,
        _IN_ uint32_t flow);

/**
 * @brief Set alarm asynchronous
 *
 * @param[in] id Device id
 * @param[in] flow Flow
 * @param[in] cb Callback
 *
 * @return Return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_flowmeter_set_alarm_async_fn) (
        _IN_ uint32_t id,
        _IN_ uint32_t flow,
        _IN_ void (cb)(uint32_t id));

/**
 * @brief flowmeter apis
 */
typedef struct _vdev_flowmeter_api_t {
    vdev_flowmeter_init_fn            init;
    vdev_flowmeter_read_fn            read;
    vdev_flowmeter_clear_fn           clear;
    vdev_flowmeter_set_alarm_fn       set_alarm;
    vdev_flowmeter_set_alarm_async_fn set_alarm_async;
} vdev_flowmeter_api_t;

#endif

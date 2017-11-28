#ifndef __VDEV_SETPMOTOR_H__
#define __VDEV_SETPMOTOR_H__

#include "vdev_types.h"


/**
 * @brief Initialize stepmotor
 *
 * @param[in] id Device id
 *
 * @return Return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_stepmotor_init_fn) (
        _IN_ uint32_t id);

/**
 * @brief Set speed
 *
 * @param[in] id Device id
 * @param[in] speed (RPM)
 *
 * @return Return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_stepmotor_set_speed_fn) (
        _IN_ uint32_t id,
        _IN_ uint32_t speed);

/**
 * @brief Set angle
 *
 * @param[in] id Device id
 * @param[in] angle Angle (rad)
 *
 * @return Return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_stepmotor_set_angle_fn) (
        _IN_ uint32_t id,
        _IN_ float angle);

/**
 * @brief Run 1 step
 *
 * @param[in] id Device id
 *
 * @return Return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_stepmotor_step_fn) (
        _IN_ uint32_t id);

/**
 * @brief stepmotor apis
 */
typedef struct _vdev_stepmotor_api_t {
    vdev_stepmotor_init_fn      init;
    vdev_stepmotor_set_speed_fn set_speed;
    vdev_stepmotor_set_angle_fn set_angle;
    vdev_stepmotor_step_fn      step;
} vdev_stepmotor_api_t;


#endif

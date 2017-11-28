#ifndef __VDEV_SETPMOTOR_H__
#define __VDEV_SETPMOTOR_H__

#include "vdev_types.h"

typedef enum _vdev_stepmotor_dir_t {
  VDEV_STEPMOTOR_DIR_CLOCKWISE,
  VDEV_STEPMOTOR_DIR_ANTICLOCKWISE,
  VDEV_STEPMOTOR_DIR_MAX
} vdev_stepmotor_dir_t;

typedef struct _vdev_stepmotor_t {
  vdev_stepmotor_dir_t dir;
  uint32_t speed;
} vdev_stepmotor_t;

/**
 * @brief Initialize stepmotor
 *
 * @param[in] id Device id
 *
 * @return Return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_stepmotor_init_fn) (
        _IN_ uint32_t id,
        _IN_ vdev_stepmotor_t *options);

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
 * @brief Set direction
 *
 * @param[in] id Device id
 * @param[in] dir Direction
 *
 * @return Return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_stepmotor_set_dir_fn) (
        _IN_ uint32_t id,
        _IN_ vdev_stepmotor_dir_t dir);

/**
 * @brief Run 1 step
 *
 * @param[in] id Device id
 * @param[in] count Step count
 *
 * @return Return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_stepmotor_step_fn) (
        _IN_ uint32_t id,
        _IN_ uint32_t count);

/**
 * @brief stepmotor apis
 */
typedef struct _vdev_stepmotor_api_t {
    vdev_stepmotor_init_fn      init;
    vdev_stepmotor_set_speed_fn set_speed;
    vdev_stepmotor_set_angle_fn set_angle;
    vdev_stepmotor_set_dir_fn   set_dir;
    vdev_stepmotor_step_fn      step;
} vdev_stepmotor_api_t;


#endif

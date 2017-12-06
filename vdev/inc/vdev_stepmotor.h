#ifndef __VDEV_SETPMOTOR_H__
#define __VDEV_SETPMOTOR_H__

#include "vdev_types.h"

typedef enum _vdev_stepmotor_dir_t {
  VDEV_STEPMOTOR_DIR_CLOCKWISE,
  VDEV_STEPMOTOR_DIR_ANTICLOCKWISE,
  VDEV_STEPMOTOR_DIR_MAX
} vdev_stepmotor_dir_t;

typedef struct _vdev_stepmotor_t {
  vdev_stepmotor_dir_t dir; /* refer to vdev_stepmotor_dir_t */
  uint32_t speed; /* RPM */
  double precision; /* stepping angle (degree), e.g. 0.9 */
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
 * @brief Run specify steps
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
 * @brief Run specify steps (asynchronous)
 *
 * @param[in] id Device id
 * @param[in] count Step count
 * @param[in] cb Callback
 * @param[in] args Callback args
 *
 * @return Return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_stepmotor_step_async_fn) (
        _IN_ uint32_t id,
        _IN_ uint32_t count,
        _IN_ void (cb)(void *args),
        _IN_ void *args);

/**
 * @brief stepmotor apis
 */
typedef struct _vdev_stepmotor_api_t {
    vdev_stepmotor_init_fn       init;
    vdev_stepmotor_set_speed_fn  set_speed;
    vdev_stepmotor_set_dir_fn    set_dir;
    vdev_stepmotor_step_fn       step;
    vdev_stepmotor_step_async_fn step_async;
} vdev_stepmotor_api_t;


#endif

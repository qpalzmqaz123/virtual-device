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
 * @brief Run 1 pulse
 *
 * @param[in] id Device id
 *
 * @return Return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_stepmotor_run_fn) (
        _IN_ uint32_t id);

/**
 * @brief stepmotor apis
 */
typedef struct _vdev_stepmotor_api_t {
    vdev_stepmotor_init_fn     init;
    vdev_stepmotor_run_fn      run;
} vdev_stepmotor_api_t;


#endif

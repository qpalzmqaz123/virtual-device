#ifndef __VDEV_H__
#define __VDEV_H__

#include "vdev_types.h"
#include "vdev_conf.h"
#include "vdev_macro.h"
#include "vdev_model.h"

#include "vdev_lcd.h"
#include "vdev_os.h"
#include "vdev_led.h"
#include "vdev_sdcard.h"
#include "vdev_stepmotor.h"


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Api id
 */
typedef enum _vdev_api_id_t {
    VDEV_API_LED = 0,
    VDEV_API_LCD,
    VDEV_API_OS,
    VDEV_API_SDCARD,
    VDEV_API_STEPMOTOR,

    VDEV_API_MAX
} vdev_api_id_t;

/**
 * @brief All virtual device apis
 */
typedef struct _vdev_api_t {
    vdev_api_id_t  id;
    vdev_model_t   model;
} vdev_api_t;

/**
 * @brief Api intiialization
 *
 * @param[in] p_api Api table
 * @param[in] count Item count in api table
 *
 * @return Return status code, refer to vdev_status_t
 */
vdev_status_t vdev_api_init(
        _IN_ vdev_api_t *p_api,
        _IN_ size_t count);

/**
 * @brief Get all api
 *
 * @param[in] id Api id
 *
 * @return Return api pointer
 */
void *vdev_api_get(
        _IN_ vdev_api_id_t id);

/**
 * @brief Set log level
 *
 * @param[in] log Log level
 *
 * @return Return status code, refer to vdev_status_t
 */
vdev_status_t vdev_set_log(
        _IN_ vdev_log_t log);

#ifdef __cplusplus
}
#endif

#endif

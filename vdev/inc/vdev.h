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
 * @brief All virtual device apis
 */
typedef struct _vdev_api_list_entry_t {
    vdev_model_t   model;
    const char *   mount_point;
} vdev_api_list_entry_t;

/**
 * @brief Api intiialization
 *
 * @param[in] p_api Api table
 *
 * @return Return status code, refer to vdev_status_t
 */
vdev_status_t
vdev_api_init(_IN_ vdev_api_list_entry_t *p_api);

/**
 * @brief Register api
 *
 * @param[in] p_api Api
 * @param[in] mount_point Mount point
 *
 */
vdev_status_t
vdev_api_register(_IN_ void  *p_api,
                  _IN_ const char *mount_point);

/**
 * @brief Get all api
 *
 * @param[in] id Api id
 *
 * @return Return api pointer
 */
void *
vdev_api_query(_IN_ const char *mount_point);

/**
 * @brief Set log level
 *
 * @param[in] log Log level
 *
 * @return Return status code, refer to vdev_status_t
 */
vdev_status_t
vdev_set_log(_IN_ vdev_log_t log);

#ifdef __cplusplus
}
#endif

#endif

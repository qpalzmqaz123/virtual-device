#ifndef __VDEV_H__
#define __VDEV_H__

#include "vdev_types.h"
#include "vdev_conf.h"
#include "vdev_macro.h"

#include "vdev_lcd.h"
#include "vdev_os.h"

/**
 * @brief all virtual device apis
 */
typedef struct _vdev_api_t {
#if VDEV_SUPPORT_LCD
    vdev_lcd_api_t      lcd;
#endif
#if VDEV_SUPPORT_OS
    vdev_os_api_t       os;
#endif
#if VDEV_SUPPORT_LOG
    vdev_log_t          log;
#endif
} vdev_api_t;

/**
 * @brief Api intiialization
 *
 * @return Return status code, refer to vdev_status_t
 */
vdev_status_t vdev_api_init(void);

/**
 * @brief Get all api
 *
 * @return Return api pointer
 */
vdev_api_t *vdev_get_api(void);

/**
 * @brief Set log level
 *
 * @return Return status code, refer to vdev_status_t
 */
vdev_status_t vdev_set_log(
        _IN_ vdev_log_t log);

#endif

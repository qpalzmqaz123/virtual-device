#ifndef __VDEV_H__
#define __VDEV_H__

#include "vdev_types.h"
#include "vdev_conf.h"
#include "vdev_macro.h"

#include "vdev_lcd.h"

/**
 * @brief all virtual device apis
 */
typedef struct _vdev_api_t {
#if VDEV_SUPPORT_LCD == 1
    vdev_lcd_api_t      lcd;
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
 * @return Return status code, refer to vdev_status_t
 */
vdev_api_t *vdev_get_api(void);

#endif

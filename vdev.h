#ifndef __VDEV_H__
#define __VDEV_H__

#include "vdev_types.h"

#include "vdev_lcd.h"

/**
 * @brief all virtual device apis
 */
typedef struct _vdev_api_t {
    vdev_lcd_api_t      lcd;
} vdev_api_t;

#endif

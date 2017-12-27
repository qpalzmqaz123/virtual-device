#ifndef __VDEV_MODEL_H__
#define __VDEV_MODEL_H__

typedef enum _vdev_model_t {
    VDEV_MODEL_NULL         = 0,

    VDEV_MODEL_GPIO         = 1,
    VDEV_MODEL_LCD          = 2,
    VDEV_MODEL_OS           = 3,
    VDEV_MODEL_LED          = 4,
    VDEV_MODEL_SDCARD       = 5,
    VDEV_MODEL_STEPMOTOR    = 6,
    VDEV_MODEL_FLOWMETER    = 7,
    VDEV_MODEL_BUTTON       = 8,

    VDEV_MODEL_MAX
} vdev_model_t;

#endif


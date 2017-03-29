#ifndef __VDEV_MODEL_H__
#define __VDEV_MODEL_H__

typedef enum _vdev_model_t {
    VDEV_MODEL_GPIO = 0,
    VDEV_MODEL_LCD,
    VDEV_MODEL_OS,
    VDEV_MODEL_LED,
    VDEV_MODEL_SDCARD,

    VDEV_MODEL_MAX
} vdev_model_t;

#endif


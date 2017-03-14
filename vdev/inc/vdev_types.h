#ifndef __VDEVTYPES_H__
#define __VDEVTYPES_H__

#include <stdint.h>

#define _IN_
#define _OUT_

typedef enum {
    VDEV_STATUS_SUCCESS = 0,
    VDEV_STATUS_FAILURE,
    VDEV_STATUS_NO_MEMORY,
} vdev_status_t;

#endif

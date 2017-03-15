#ifndef __VDEVTYPES_H__
#define __VDEVTYPES_H__

#include <stdint.h>

typedef uint32_t BOOL;

#define TRUE 1
#define FALSE 0

#define _IN_
#define _OUT_

typedef enum {
    VDEV_STATUS_SUCCESS = 0,
    VDEV_STATUS_FAILURE,
    VDEV_STATUS_NO_MEMORY,
    VDEV_STATUS_TIMEOUT,
    VDEV_STATUS_EMPTY,
    VDEV_STATUS_FULL
} vdev_status_t;

#endif

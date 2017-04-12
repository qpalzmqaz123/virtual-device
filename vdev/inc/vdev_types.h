#ifndef __VDEVTYPES_H__
#define __VDEVTYPES_H__

#include <stdint.h>

typedef uint32_t bool;

#define TRUE 1
#define FALSE 0

#define _IN_
#define _OUT_

#define VDEV_ID_ALL (-1) /* you can use VDEV_ID_ALL to initialize all device */

typedef enum _vdev_status_t {
    VDEV_STATUS_SUCCESS = 0,
    VDEV_STATUS_FAILURE,
    VDEV_STATUS_NO_MEMORY,
    VDEV_STATUS_TIMEOUT,
    VDEV_STATUS_EMPTY,
    VDEV_STATUS_FULL,
    VDEV_STATUS_NOT_EXIST
} vdev_status_t;

typedef enum _vdev_log_t {
    VDEV_LOG_DEBUG = 0,
    VDEV_LOG_INFO,
    VDEV_LOG_WARING,
    VDEV_LOG_ERROR,
    VDEV_LOG_CRITICAL
} vdev_log_t;

#endif

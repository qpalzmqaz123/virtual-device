#ifndef __VDEV_MACRO_H__
#define __VDEV_MACRO_H__

#include "vdev_conf.h"

#define VDEV_ASSERT_NOT_NULL(ptr) do { \
    if (NULL == ptr) { \
        vprintf("%% ASSERT_NOT_NULL failed at %s: %d\n", __FILE__, __LINE__); \
        exit(127); \
    } \
} while(0)

#define VDEV_ASSERT_SUCCESS(ret) do { \
    if (ret) { \
        vprintf("%% ASSERT_SUCCESS failed at %s: %d\n", __FILE__, __LINE__); \
        exit(127); \
    } \
} while(0)

#endif

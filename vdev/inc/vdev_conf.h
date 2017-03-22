#ifndef __VDEV_CONF_H__
#define __VDEV_CONF_H__

#define VDEV_SUPPORT_LOG   1

#define VDEV_SUPPORT_LCD   1
#define VDEV_SUPPORT_TOUCH 1 /* require SUPPORT_LCD */
#define VDEV_SUPPORT_OS    1

#define VDEV_SIMULATION_TYPE  1 /* 0: Disable, 1: Posix, 2: Windows */

#include <stdio.h>
#include <stdlib.h>

#define VDEV_LOG_PRESENT(fmt, ...) do { \
    printf(fmt, ##__VA_ARGS__); \
} while (0)

#define VDEV_ABORT() do { \
    abort(); \
} while (0)

#endif

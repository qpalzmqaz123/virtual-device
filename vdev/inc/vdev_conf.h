#ifndef __VDEV_CONF_H__
#define __VDEV_CONF_H__


#define VDEV_SUPPORT_LOG   0


#ifdef VDEV_SIMULATION_TYPE_POSIX
#define VDEV_SIMULATION_TYPE  1 /* 0: Disable, 1: Posix */
#else
#define VDEV_SIMULATION_TYPE  0
#endif

#define VDEV_SIMULATION_SOCKET_PATH "/tmp/vdev_socket"

#define VDEV_SIMULATION_SDCARD_CAPACITY  (1000 * 1000 * 1000)  /* 1G */
#define VDEV_SIMULATION_SDCARD_BLOCKSIZE 512

#include <stdio.h>
#include <stdlib.h>

#define VDEV_LOG_PRESENT(fmt, ...) do { \
    printf(fmt, ##__VA_ARGS__); \
} while (0)

#define VDEV_ABORT() do { \
    abort(); \
} while (0)

#endif

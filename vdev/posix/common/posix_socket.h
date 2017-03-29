#ifndef __POSIX_SOCKET_H__
#define __POSIX_SOCKET_H__

#include "vdev_types.h"

void posix_socket_init(void);
uint32_t posix_socket_send(void *data, uint32_t length);
uint32_t posix_socket_recv(void *data, uint32_t length);

#endif


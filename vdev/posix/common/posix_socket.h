#ifndef __POSIX_SOCKET_H__
#define __POSIX_SOCKET_H__

#include "vdev_types.h"

int posix_socket_server_init(int client_num, const char *socket_file);
int posix_socket_client_init(const char *socket_file);
int posix_socket_accept(int srv);
int posix_socket_write(int sock, void *buff, int len);
int posix_socket_read(int sock, void *buff, int len);

#endif


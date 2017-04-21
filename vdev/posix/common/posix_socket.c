#include "posix_socket.h"
#include "vdev_conf.h"
#include "vdev_macro.h"
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


int
posix_socket_server_init(int client_num, const char *socket_file)
{
    int fd;
    struct sockaddr_un saun;

    VDEV_ASSERT((fd = socket(AF_UNIX, SOCK_STREAM, 0)) != -1);

    saun.sun_family = AF_UNIX;
    strcpy(saun.sun_path, socket_file);

    unlink(socket_file);
    VDEV_ASSERT(bind(fd, (struct sockaddr *)&saun, sizeof(struct sockaddr_un)) != -1);

    VDEV_ASSERT(listen(fd, client_num) != -1);

    return fd;
}


int
posix_socket_client_init(const char *socket_file)
{
    int fd;
    struct sockaddr_un saun;

    VDEV_ASSERT((fd = socket(AF_UNIX, SOCK_STREAM, 0)) != -1);

    saun.sun_family = AF_UNIX;
    strcpy(saun.sun_path, socket_file);

    while (1) {
        if (-1 != connect(fd, (struct sockaddr *)&saun, sizeof(struct sockaddr_un))) {
            break;
        }
        usleep(100000);
    }

    return fd;
}

int
posix_socket_accept(int srv)
{
    int conn;

    VDEV_ASSERT((conn = accept(srv, NULL, NULL)) != -1);

    return conn;
}


int
posix_socket_write(int sock, void *buff, int size)
{
    return write(sock, buff, size);
}


int
posix_socket_read(int sock, void *buff, int size)
{
    return read(sock, buff, size);
}


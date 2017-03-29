#include "posix_socket.h"
#include "vdev_conf.h"
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

static int Sock;
static pthread_mutex_t Mutex = PTHREAD_MUTEX_INITIALIZER;

void posix_socket_init(void)
{
    int len;
    struct sockaddr_un saun;

    Sock = socket(AF_UNIX, SOCK_STREAM, 0);
    saun.sun_family = AF_UNIX;
    strcpy(saun.sun_path, VDEV_SIMULATION_SOCKET_PATH);
    len = sizeof(saun.sun_family) + strlen(saun.sun_path);

    printf("Try to connect server");
    while (1) {
        fflush(stdout);
        if (0 == connect(Sock, (struct sockaddr *)&saun, len)) break;
        printf(".");
        sleep(1);
    }
    printf("\nConnection established\n");
}

uint32_t posix_socket_send(void *data, uint32_t length)
{
    uint32_t len;

    pthread_mutex_lock(&Mutex);
    len = send(Sock, data, length, 0);
    pthread_mutex_unlock(&Mutex);

    return len;
}

uint32_t posix_socket_recv(void *data, uint32_t length)
{
    return 0;
}


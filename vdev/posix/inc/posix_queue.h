#ifndef __POSIX_QUEUE_H__
#define __POSIX_QUEUE_H__

#include <unistd.h>
#include <pthread.h>

typedef struct _posix_queue_node_t {
    void *data;
    struct _posix_queue_node_t *next;
} posix_queue_node_t;

typedef struct _posix_queue_t {
    posix_queue_node_t *head;
    posix_queue_node_t *tail;
    int count;
    int length;
    int itemsize;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} posix_queue_t;

typedef enum _posix_queue_error_t {
    POSIX_QUEUE_SUCCESS = 0,
    POSIX_QUEUE_FAILURE,
    POSIX_QUEUE_NO_MEMORY,
    POSIX_QUEUE_TIMEOUT,
} posix_queue_error_t;

posix_queue_error_t posix_queue_create(posix_queue_t *queue, int length, int itemsize);
posix_queue_error_t posix_queue_put(posix_queue_t *queue, void *item);
posix_queue_error_t posix_queue_get(posix_queue_t *queue, void *item);
posix_queue_error_t posix_queue_put_timeout(posix_queue_t *queue, void *item, int ms);
posix_queue_error_t posix_queue_get_timeout(posix_queue_t *queue, void *item, int ms);

#endif

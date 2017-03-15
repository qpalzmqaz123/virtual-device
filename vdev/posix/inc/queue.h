#ifndef __POSIX_QUEUE_H__
#define __POSIX_QUEUE_H__

#include <pthread.h>

typedef struct _queue_t {
    uint32_t count;
    void **head;
    void **p1; /* read cursor */
    void **p2; /* push cursor */
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} queue_t;

vdev_status_t posix_queue_init(queue_t *q, uint32_t count);
vdev_status_t posix_queue_put(queue_t *q, const void *item, uint32_t item_size);
vdev_status_t posix_queue_get(queue_t *q, void *item, uint32_t item_size);
BOOL posix_queue_full(queue_t *q);
BOOL posix_queue_empty(queue_t *q);
vdev_status_t posix_queue_destroy(queue_t *q);

#endif

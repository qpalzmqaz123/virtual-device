#include "vdev.h"
#include "queue.h"
#include <stdlib.h>
#include <string.h>

vdev_status_t posix_queue_init(queue_t *q, uint32_t count)
{
    memset(q, 0, sizeof(queue_t));

    q->head = (void **)malloc(sizeof(void *) * (count + 1));
    if (NULL == q->head) {
        return VDEV_STATUS_NO_MEMORY;
    }
    q->p2 = q->p1 = q->head;
    q->count = count;

    memset(&q->mutex, 0, sizeof(pthread_mutex_t));
    memset(&q->cond, 0, sizeof(pthread_cond_t));

    return VDEV_STATUS_SUCCESS;
}

vdev_status_t posix_queue_put(queue_t *q, const void *item, uint32_t item_size)
{
    uint32_t item_num;

    pthread_mutex_lock(&q->mutex);
    /* full? */
    if (q->p2 - q->head == q->count && q->p1 == q->head) {
        pthread_mutex_unlock(&q->mutex);
        return VDEV_STATUS_FULL;
    }

    /* clean up queue */
    item_num = q->p2 - q->p1;
    if (q->p2 - q->head == q->count) {
        memmove(q->p1, q->head, item_num);
        q->p1 = q->head;
        q->p2 = q->head + item_num;
    }
    /* push item */
    *q->p2 = (void *)malloc(item_size);
    if (NULL == *q->p2) {
        return VDEV_STATUS_NO_MEMORY;
    }
    memcpy(*q->p2, item, item_size);
    q->p2++;

    pthread_mutex_unlock(&q->mutex);

    return VDEV_STATUS_SUCCESS;
}

vdev_status_t posix_queue_get(queue_t *q, void *item, uint32_t item_size)
{
    pthread_mutex_lock(&q->mutex);
    /* empty? */
    if (q->p2 == q->p1) {
        pthread_mutex_unlock(&q->mutex);
        return VDEV_STATUS_EMPTY;
    }
    /* get item */
    memcpy(item, *q->p1, item_size);
    q->p1++;

    pthread_mutex_unlock(&q->mutex);

    return VDEV_STATUS_SUCCESS;
}

bool posix_queue_empty(queue_t *q)
{
    bool res;
    pthread_mutex_lock(&q->mutex);
    res = (q->p2 == q->p1);
    pthread_mutex_unlock(&q->mutex);

    return res;
}

bool posix_queue_full(queue_t *q)
{
    bool res;
    pthread_mutex_lock(&q->mutex);
    res = (q->p2 - q->head == q->count && q->p1 == q->head);
    pthread_mutex_unlock(&q->mutex);

    return res;
}

vdev_status_t posix_queue_destroy(queue_t *q)
{
    void **pp;

    if (q->p2 - q->p1 > 0) {
        for (pp = q->p1; pp != q->p2; pp++) {
            free(*pp);
        }
    }
    free(q->head);

    pthread_mutex_destroy(&q->mutex);
    pthread_cond_destroy(&q->cond);

    return VDEV_STATUS_SUCCESS;
}


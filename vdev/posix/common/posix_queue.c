#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "posix_queue.h"


static posix_queue_error_t
_posix_queue_put(posix_queue_t *queue, void *item, int ms, int timeout_en)
{
    posix_queue_error_t error = POSIX_QUEUE_SUCCESS;
    posix_queue_node_t *node;
    struct timeval now;
    struct timeval wait;
    struct timeval to_wait;
    struct timespec to_wait_r;

    /* calculate timeout */
    if (timeout_en) {
        gettimeofday(&now, NULL);
        wait.tv_sec = ms / 1000;
        wait.tv_usec = (ms % 1000) * 1000;
        timeradd(&now, &wait, &to_wait);
        to_wait_r.tv_sec = to_wait.tv_sec;
        to_wait_r.tv_nsec = to_wait.tv_usec * 1000;
    }

    pthread_mutex_lock(&queue->mutex);

    /* wait event */
    if (queue->count == queue->length) {
        if (timeout_en) {
            if (pthread_cond_timedwait(&queue->cond, &queue->mutex, &to_wait_r)) {
                error = POSIX_QUEUE_TIMEOUT; 
                goto RET;
            }
        }
        else {
            pthread_cond_wait(&queue->cond, &queue->mutex);
        }
    }

    node = (posix_queue_node_t *)malloc(sizeof(posix_queue_node_t));
    if (NULL == node) {
        error = POSIX_QUEUE_OUT_OF_MEMORY;
        goto RET;
    }
    node->data = (void *)malloc(sizeof(queue->itemsize));
    if (NULL == node->data) {
        free(node);
        error = POSIX_QUEUE_OUT_OF_MEMORY;
        goto RET;
    }
    memcpy(node->data, item, sizeof(queue->itemsize));
    node->next = NULL;

    if (0 == queue->count) {
        queue->head = node;
        queue->tail = node;
    }
    else {
        queue->tail->next = node;
        queue->tail = node;
    }

    queue->count++;

    pthread_cond_signal(&queue->cond);
RET:
    pthread_mutex_unlock(&queue->mutex);

    return error;
}

static posix_queue_error_t
_posix_queue_get(posix_queue_t *queue, void *item, int ms, int timeout_en)
{
    posix_queue_error_t error = POSIX_QUEUE_SUCCESS;
    posix_queue_node_t *node;
    struct timeval now;
    struct timeval wait;
    struct timeval to_wait;
    struct timespec to_wait_r;

    /* calculate timeout */
    if (timeout_en) {
        gettimeofday(&now, NULL);
        wait.tv_sec = ms / 1000;
        wait.tv_usec = (ms % 1000) * 1000;
        timeradd(&now, &wait, &to_wait);
        to_wait_r.tv_sec = to_wait.tv_sec;
        to_wait_r.tv_nsec = to_wait.tv_usec * 1000;
    }

    pthread_mutex_lock(&queue->mutex);

    if (queue->count == 0) {
        if (timeout_en) {
            if (pthread_cond_timedwait(&queue->cond, &queue->mutex, &to_wait_r)) {
                error = POSIX_QUEUE_TIMEOUT; 
                goto RET;
            }
        }
        else {
            pthread_cond_wait(&queue->cond, &queue->mutex);
        }
    }

    memcpy(item, queue->head->data, sizeof(queue->itemsize));
    free(queue->head->data);
    node = queue->head->next;
    free(queue->head);
    queue->head = node;

    queue->count--;

    pthread_cond_signal(&queue->cond);
RET:
    pthread_mutex_unlock(&queue->mutex);

    return error;
}

posix_queue_error_t
posix_queue_create(posix_queue_t *queue, int length, int itemsize)
{
    int res;

    queue->count = 0;
    queue->length = length;
    queue->itemsize = itemsize;

    res = pthread_mutex_init(&queue->mutex, NULL);
    res += pthread_cond_init(&queue->cond, NULL);

    if (res) return POSIX_QUEUE_FAILURE;

    return POSIX_QUEUE_SUCCESS;
}

posix_queue_error_t
posix_queue_delete(posix_queue_t *queue)
{
    posix_queue_node_t *node = queue->head;

    pthread_mutex_lock(&queue->mutex);

    while (queue->count--) {
        free(node->data);
        free(node);
        node = node->next;
    }

    pthread_mutex_unlock(&queue->mutex);
    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->cond);

    return POSIX_QUEUE_SUCCESS;
}

posix_queue_error_t
posix_queue_put(posix_queue_t *queue, void *item)
{
    return _posix_queue_put(queue, item, 0, 0);
}

posix_queue_error_t
posix_queue_get(posix_queue_t *queue, void *item)
{
    return _posix_queue_get(queue, item, 0, 0);
}

posix_queue_error_t
posix_queue_put_timeout(posix_queue_t *queue, void *item, int ms)
{
    return _posix_queue_put(queue, item, ms, 1);
}

posix_queue_error_t
posix_queue_get_timeout(posix_queue_t *queue, void *item, int ms)
{
    return _posix_queue_get(queue, item, ms, 1);
}

#if 0 /* test */

static void *
producer(void *arg)
{
    int i;
    int rc;
    posix_queue_t *queue = (posix_queue_t *)arg;

    for (i = 0; i < 10; i++) {
        printf("put: %d\n", i);
        rc = posix_queue_put_timeout(queue, &i, 400);
        if (POSIX_QUEUE_TIMEOUT == rc) {
            printf("put: timeout!\n");
            i--;
            continue;
        }
    }

    return NULL;
}

static void *
consumer1(void *arg) {
    posix_queue_t *queue = (posix_queue_t *)arg;
    int num;
    int rc;

    while (1) {
        usleep(500000);
        rc = posix_queue_get_timeout(queue, &num, 500000);
        if (POSIX_QUEUE_TIMEOUT == rc) {
            printf("c1: timeout!\n");
            break;
        }
        printf("c1 get: %d\n", num);
    }

    return NULL;
}

static void *
consumer2(void *arg)
{
    posix_queue_t *queue = (posix_queue_t *)arg;
    int num;
    int rc;

    while (1) {
        sleep(1);
        rc = posix_queue_get_timeout(queue, &num, 1000);
        if (POSIX_QUEUE_TIMEOUT == rc) {
            printf("c2: timeout!\n");
            break;
        }
        printf("c2 get: %d\n", num);
    }

    return NULL;
}

void
test_posix_queue(void)
{
    pthread_t prod, cons1, cons2;
    posix_queue_t queue;

    posix_queue_create(&queue, 5, sizeof(int));

    pthread_create(&prod, NULL, producer, (void *)&queue);
    pthread_create(&cons1, NULL, consumer1, (void *)&queue);
    pthread_create(&cons2, NULL, consumer2, (void *)&queue);

    getchar();

    posix_queue_delete(&queue);
}
#endif

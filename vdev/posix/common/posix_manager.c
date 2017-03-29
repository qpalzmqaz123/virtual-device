#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "posix_manager.h"
#include "posix_socket.h"

typedef struct _posix_message_header_t {
    uint32_t model;
    uint32_t id;
    uint32_t length; /* data length, exclued header */
} posix_message_header_t;

typedef struct _posix_manager_queue_table_t {
    posix_queue_t **queue;
    uint32_t count;
} posix_manager_queue_table_t;


/* TODO: Consider replace QueueTbl with hash */
static posix_manager_queue_table_t QueueTbl[VDEV_MODEL_MAX];

posix_manager_model_t *
posix_manager_register(vdev_model_t model, uint32_t id)
{
    posix_manager_model_t *p = NULL;

    p = (posix_manager_model_t *)malloc(sizeof(posix_manager_model_t));
    if (NULL == p) return NULL;

    p->model = model;
    p->id = id;

    posix_queue_create(&p->queue, 1024, sizeof(posix_manager_message_t));

    /* save queue */
    if (NULL == QueueTbl[model].queue) {
        QueueTbl[model].count = id + 1;
        QueueTbl[model].queue = (posix_queue_t **)malloc(sizeof(posix_queue_t *) * QueueTbl[model].count);
    }
    else {
        if (id > QueueTbl[model].count + 1) {
            QueueTbl[model].count = id + 1;
            QueueTbl[model].queue = (posix_queue_t **)realloc(QueueTbl[model].queue, sizeof(posix_queue_t *) * QueueTbl[model].count);
        }
    }
    QueueTbl[model].queue[id] = &p->queue;

    return p;
}

uint32_t
posix_manager_send(posix_manager_model_t *model, void *data, uint32_t length)
{
    uint32_t len;

    void *p = malloc(sizeof(posix_message_header_t) + length);

    /* encapsulate data */
    ((posix_message_header_t *)p)->model  = model->model;
    ((posix_message_header_t *)p)->id     = model->id;
    ((posix_message_header_t *)p)->length = length;
    memcpy(p + sizeof(posix_message_header_t), data, length);

    len = posix_socket_send(p, sizeof(posix_message_header_t) + length);
    free(p);

    return len;
}

uint32_t
posix_manager_recv(posix_manager_model_t *model, void *data, uint32_t length)
{
    return length;
}

void
posix_manager_init(void)
{
    memset(&QueueTbl, 0, sizeof(QueueTbl));
    posix_socket_init();
}


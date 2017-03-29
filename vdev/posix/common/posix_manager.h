#ifndef __POSIX_MANAGER_H__
#define __POSIX_MANAGER_H__

#include "vdev_types.h"
#include "vdev_model.h"
#include "posix_queue.h"

typedef struct _posix_manager_model_t {
    vdev_model_t model;
    uint32_t id;
    posix_queue_t queue;
} posix_manager_model_t;

typedef struct _posix_manager_message_t {
    void *data;
    uint32_t length; /* data length */
    uint32_t time;
} posix_manager_message_t;

void
posix_manager_init(void);

posix_manager_model_t *
posix_manager_register(vdev_model_t model, uint32_t id);

uint32_t
posix_manager_send(posix_manager_model_t *model, void *data, uint32_t length);

uint32_t
posix_manager_recv(posix_manager_model_t *model, void *data, uint32_t length);

#endif


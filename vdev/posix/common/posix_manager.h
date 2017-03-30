#ifndef __POSIX_MANAGER_H__
#define __POSIX_MANAGER_H__

#include "vdev_types.h"
#include "vdev_model.h"

#define POSIX_MANAGER_RECV_BUFFER_BLOCK_SIZE 50

typedef struct _posix_manager_key_t {
    vdev_model_t model;
    uint32_t     id;
} posix_manager_key_t;

void
posix_manager_init(void);

uint32_t
posix_manager_register(const posix_manager_key_t *key);

uint32_t
posix_manager_send(const posix_manager_key_t *key, void *data, uint32_t length);

uint32_t
posix_manager_recv(const posix_manager_key_t *key, void *data, uint32_t length);

#endif


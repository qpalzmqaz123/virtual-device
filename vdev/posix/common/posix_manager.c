#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "posix_manager.h"
#include "posix_socket.h"
#include "uthash/include/uthash.h"
#include "vdev.h"


#define HASH_SAVE_ITEM(item_ptr) do { \
    pthread_rwlock_wrlock(&Lock); \
    HASH_ADD(hh, pHead, key, sizeof(posix_manager_key_t), (item_ptr)); \
    pthread_rwlock_unlock(&Lock); \
} while(0)

#define HASH_GET_ITEM(key_ptr, item_ptr) do { \
    pthread_rwlock_rdlock(&Lock); \
    HASH_FIND(hh, pHead, (key_ptr), sizeof(posix_manager_key_t), (item_ptr)); \
    pthread_rwlock_unlock(&Lock); \
} while(0)


typedef struct _posix_message_header_t {
    uint32_t model;
    uint32_t id;
    uint32_t length; /* data length, exclued header */
} posix_message_header_t;

typedef struct _posix_manager_item_t {
    posix_manager_key_t key;
    UT_hash_handle      hh;
    void                *buffer; /* receive buffer */
    uint32_t            rd;      /* read pointer offset */
    uint32_t            wr;      /* write pointer offset */
    uint32_t            block;   /* block number */
    pthread_mutex_t     mutex;
    pthread_cond_t      cond;
} posix_manager_item_t;


static posix_manager_item_t *pHead = NULL;
static pthread_rwlock_t Lock = PTHREAD_RWLOCK_INITIALIZER;
static int Sock;


uint32_t
posix_manager_register(const posix_manager_key_t *key)
{
    posix_manager_item_t *item;

    item = (posix_manager_item_t *)malloc(sizeof(posix_manager_item_t));
    memset(item, 0, sizeof(posix_manager_item_t));

    pthread_mutex_init(&item->mutex, NULL);
    pthread_cond_init(&item->cond, NULL);

    /* assign key */
    memcpy(&item->key, key, sizeof(posix_manager_key_t));

    /* save item */
    HASH_SAVE_ITEM(item);

    return 0;
}

uint32_t
posix_manager_send(const posix_manager_key_t *key, void *data, uint32_t length)
{
    uint32_t len = 0;

    void *p = malloc(sizeof(posix_message_header_t) + length);

    /* encapsulate data */
    ((posix_message_header_t *)p)->model  = key->model;
    ((posix_message_header_t *)p)->id     = key->id;
    ((posix_message_header_t *)p)->length = length;
    memcpy(p + sizeof(posix_message_header_t), data, length);

    len = posix_socket_write(Sock, p, sizeof(posix_message_header_t) + length);
    free(p);

    return len;
}

static void
posix_recv_read_buffer(posix_manager_item_t *item, void *data, uint32_t length);

uint32_t
posix_manager_recv(const posix_manager_key_t *key, void *data, uint32_t length)
{
    posix_manager_item_t *item;

    /* lookup item */
    HASH_GET_ITEM(key, item);

    posix_recv_read_buffer(item, data, length);

    return 0;
}

static void
posix_recv_read_buffer(posix_manager_item_t *item, void *data, uint32_t length)
{
    if (0 == length) return;

    pthread_mutex_lock(&item->mutex);

    /* wait signal */
    while (length > (item->wr - item->rd)) {
        pthread_cond_wait(&item->cond, &item->mutex);
    }

    /*  copy data */
    memcpy(data, item->buffer + item->rd, length);
    item->rd += length;

    /* if buffer is empty, release the memory */
    if (item->rd == item->wr) {
        free(item->buffer);
        item->buffer = NULL;
    }

    pthread_mutex_unlock(&item->mutex);
}

static void
posix_recv_write_buffer(posix_manager_item_t *item, void *data, uint32_t length)
{
    uint32_t block_num = 0;

    if (0 == length) return;

    pthread_mutex_lock(&item->mutex);

    if (NULL == item->buffer) {
        block_num = (length - 1) / POSIX_MANAGER_RECV_BUFFER_BLOCK_SIZE + 1;
        item->buffer = malloc(block_num * POSIX_MANAGER_RECV_BUFFER_BLOCK_SIZE);
        VDEV_ASSERT_NOT_NULL(item->buffer);
        memcpy(item->buffer, data, length);

        item->rd = 0;
        item->wr = length;
        item->block = block_num;
    }
    else {
        block_num = (item->wr + length - 1) / POSIX_MANAGER_RECV_BUFFER_BLOCK_SIZE + 1;
        if (block_num != item->block) {
            item->buffer = realloc(item->buffer, block_num * POSIX_MANAGER_RECV_BUFFER_BLOCK_SIZE);
            VDEV_ASSERT_NOT_NULL(item->buffer);
            item->block = block_num;
        }
        memcpy(item->buffer + item->wr, data, length);

        item->wr += length;
    }

    pthread_cond_signal(&item->cond);
    pthread_mutex_unlock(&item->mutex);
}

static void *
posix_recv_loop(void *arg)
{
    posix_message_header_t header;
    posix_manager_key_t key;
    posix_manager_item_t *item;
    void *data;
    uint32_t len;

    while (1) {
        item = NULL;

        /* receive header */
        len = posix_socket_read(Sock, &header, sizeof(posix_message_header_t));
        VDEV_ASSERT(len == sizeof(posix_message_header_t));

        /* receive data */
        data = malloc(header.length);
        len = posix_socket_read(Sock, data, header.length);
        VDEV_ASSERT(len == header.length);

        /* lookup item */
        key.model = header.model;
        key.id = header.id;

        HASH_GET_ITEM(&key, item);
        if (NULL == item) goto NEXT;

        /* distribute data */
        posix_recv_write_buffer(item, data, header.length);

NEXT:
        free(data);
    }

    return NULL;
}

void
posix_manager_init(void)
{
    pthread_t thread;

    /* connect to server */
    printf("Try to connect backend...\n");
    Sock = posix_socket_client_init(VDEV_SIMULATION_SOCKET_PATH);
    printf("Connection established\n");

    /* create a new thread to distribute the received data */
    pthread_create(&thread, NULL, posix_recv_loop, NULL);
}


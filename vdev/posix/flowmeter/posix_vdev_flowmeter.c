#include "vdev.h"
#include "vdev_model.h"
#include "posix_manager.h"
#include "uthash/include/uthash.h"

typedef enum _flowmeter_cmd_t {
    FLOWMETER_CMD_INIT      = 0,
    FLOWMETER_CMD_READ      = 1,
    FLOWMETER_CMD_CLEAR     = 2,
    FLOWMETER_CMD_SET_ALARM = 3
} flowmeter_cmd_t;

typedef struct _flowmeter_t {
    int                 id;
    posix_manager_key_t key;
    UT_hash_handle      hh;
} flowmeter_t;

typedef struct _flowmeter_async_t {
    void (*cb)(void *args);
    void *args;
} flowmeter_async_t;

static flowmeter_t *pHead = NULL;

static void
posix_vdev_flowmeter_async_callback(const void *data, uint32_t length, void *args)
{
    flowmeter_async_t *async = (flowmeter_async_t *)args;

    async->cb(async->args);

    free(async);
}

static vdev_status_t
posix_vdev_flowmeter_init(
        _IN_ uint32_t id)
{
    uint8_t res;
    flowmeter_t *p_flowmeter;
    uint8_t cmd = FLOWMETER_CMD_INIT;

    VDEV_RETURN_IF_NULL(p_flowmeter = (flowmeter_t *)malloc(sizeof(flowmeter_t)), VDEV_STATUS_OUT_OF_MEMORY, "");
    memset(p_flowmeter, 0, sizeof(flowmeter_t));

    p_flowmeter->id = id;
    p_flowmeter->key.id = id;
    p_flowmeter->key.model = VDEV_MODEL_FLOWMETER;
    HASH_ADD_INT(pHead, id, p_flowmeter);

    posix_manager_register(&p_flowmeter->key);

    posix_manager_send(&p_flowmeter->key, &cmd, sizeof(uint8_t));
    posix_manager_recv(&p_flowmeter->key, &res, sizeof(uint8_t));

    return res;
}

static vdev_status_t
posix_vdev_flowmeter_read(
        _IN_ uint32_t id,
        _OUT_ uint32_t *flow)
{
    flowmeter_t *p_flowmeter;
    uint8_t msg[1];
    uint8_t res[5];

    HASH_FIND_INT(pHead, &id, p_flowmeter);
    VDEV_RETURN_IF_NULL(p_flowmeter, VDEV_STATUS_FAILURE, "");

    *msg = FLOWMETER_CMD_READ;
    posix_manager_send(&p_flowmeter->key, msg, sizeof(msg));
    posix_manager_recv(&p_flowmeter->key, res, sizeof(res));

    memcpy(flow, res + 1, 4);

    return res[0];
}

static vdev_status_t
posix_vdev_flowmeter_clear(
        _IN_ uint32_t id)
{
    flowmeter_t *p_flowmeter;
    uint8_t msg[1];
    uint8_t res[1];

    HASH_FIND_INT(pHead, &id, p_flowmeter);
    VDEV_RETURN_IF_NULL(p_flowmeter, VDEV_STATUS_FAILURE, "");

    *msg = FLOWMETER_CMD_CLEAR;
    posix_manager_send(&p_flowmeter->key, msg, sizeof(msg));
    posix_manager_recv(&p_flowmeter->key, res, sizeof(res));

    return res[0];
}

static vdev_status_t
posix_vdev_flowmeter_set_alarm(
        _IN_ uint32_t id,
        _IN_ uint32_t flow)
{
    flowmeter_t *p_flowmeter;
    uint8_t msg[5];
    uint8_t res[1];

    HASH_FIND_INT(pHead, &id, p_flowmeter);
    VDEV_RETURN_IF_NULL(p_flowmeter, VDEV_STATUS_FAILURE, "");

    *msg = FLOWMETER_CMD_SET_ALARM;
    memcpy(msg + 1, &flow, 4);
    posix_manager_send(&p_flowmeter->key, msg, sizeof(msg));
    posix_manager_recv(&p_flowmeter->key, res, sizeof(res));

    return res[0];
}

static vdev_status_t
posix_vdev_flowmeter_set_alarm_async(
        _IN_ uint32_t id,
        _IN_ uint32_t flow,
        _IN_ void (*cb)(void *args),
        _IN_ void *args)
{
    flowmeter_t *p_flowmeter;
    uint8_t msg[5];
    flowmeter_async_t *async;

    HASH_FIND_INT(pHead, &id, p_flowmeter);
    VDEV_RETURN_IF_NULL(p_flowmeter, VDEV_STATUS_FAILURE, "");

    async = (flowmeter_async_t *)malloc(sizeof(flowmeter_async_t));
    async->cb = cb;
    async->args = args;

    *msg = FLOWMETER_CMD_SET_ALARM;
    memcpy(msg + 1, &flow, 4);
    posix_manager_send(&p_flowmeter->key, msg, sizeof(msg));
    posix_manager_recv_async(&p_flowmeter->key, 1, posix_vdev_flowmeter_async_callback, async);

    return VDEV_STATUS_SUCCESS;
}

void
vdev_flowmeter_api_install(vdev_flowmeter_api_t *api)
{
    api->init = posix_vdev_flowmeter_init;
    api->read = posix_vdev_flowmeter_read;
    api->clear = posix_vdev_flowmeter_clear;
    api->set_alarm = posix_vdev_flowmeter_set_alarm;
    api->set_alarm_async = posix_vdev_flowmeter_set_alarm_async;
}

#include "vdev.h"
#include "vdev_model.h"
#include "posix_manager.h"
#include "uthash.h"


typedef enum _stepmotor_cmd {
    STEPMOTOR_CMD_INIT   = 0,
    STEPMOTOR_CMD_RUN    = 1
} stepmotor_cmd;

typedef struct _stepmotor_t {
    int                 id;
    posix_manager_key_t key;
    UT_hash_handle      hh;
} stepmotor_t;


static stepmotor_t *pHead = NULL;


vdev_status_t posix_vdev_stepmotor_init(
        _IN_ uint32_t id)
{
    stepmotor_t *p_motor = NULL;
    uint8_t cmd = STEPMOTOR_CMD_INIT;
    uint32_t res;

    VDEV_RETURN_IF_NULL(p_motor = (stepmotor_t *)malloc(sizeof(stepmotor_t)), VDEV_STATUS_NO_MEMORY, "");
    memset(p_motor, 0, sizeof(stepmotor_t));

    p_motor->id = id;
    p_motor->key.id = id;
    p_motor->key.model = VDEV_MODEL_STEPMOTOR;
    HASH_ADD_INT(pHead, id, p_motor);

    posix_manager_register(&p_motor->key);

    posix_manager_send(&p_motor->key, &cmd, sizeof(uint8_t));
    posix_manager_recv(&p_motor->key, &res , sizeof(uint32_t));

    return res;
}

vdev_status_t posix_vdev_stepmotor_run(
        _IN_ uint32_t id)
{
    stepmotor_t *p_motor = NULL;
    uint8_t cmd = STEPMOTOR_CMD_RUN;
    uint32_t res;

    HASH_FIND_INT(pHead, &id, p_motor);
    VDEV_RETURN_IF_NULL(p_motor, VDEV_STATUS_FAILURE, "");

    posix_manager_send(&p_motor->key, &cmd, sizeof(uint8_t));
    posix_manager_recv(&p_motor->key, &res , sizeof(uint32_t));

    return res;
}

void vdev_stepmotor_api_install(vdev_stepmotor_api_t *api)
{
    api->init   = posix_vdev_stepmotor_init;
    api->run    = posix_vdev_stepmotor_run;
}


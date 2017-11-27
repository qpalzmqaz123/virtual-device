#include "vdev.h"
#include "vdev_model.h"
#include "posix_manager.h"
#include "uthash/include/uthash.h"


typedef enum _stepmotor_cmd_t {
    STEPMOTOR_CMD_INIT      = 0,
    STEPMOTOR_CMD_SET_SPEED = 1,
    STEPMOTOR_CMD_SET_ANGLE = 2,
    STEPMOTOR_CMD_STEP      = 3
} stepmotor_cmd_t;

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
    uint8_t res;

    VDEV_RETURN_IF_NULL(p_motor = (stepmotor_t *)malloc(sizeof(stepmotor_t)), VDEV_STATUS_OUT_OF_MEMORY, "");
    memset(p_motor, 0, sizeof(stepmotor_t));

    p_motor->id = id;
    p_motor->key.id = id;
    p_motor->key.model = VDEV_MODEL_STEPMOTOR;
    HASH_ADD_INT(pHead, id, p_motor);

    posix_manager_register(&p_motor->key);

    posix_manager_send(&p_motor->key, &cmd, sizeof(uint8_t));
    posix_manager_recv(&p_motor->key, &res , sizeof(uint8_t));

    return (vdev_status_t)res;
}

vdev_status_t posix_vdev_stepmotor_set_speed(
        _IN_ uint32_t id,
        _IN_ uint32_t speed)
{
    stepmotor_t *p_motor = NULL;
    uint8_t msg[sizeof(uint8_t) + sizeof(uint32_t)];
    uint8_t res;

    HASH_FIND_INT(pHead, &id, p_motor);
    VDEV_RETURN_IF_NULL(p_motor, VDEV_STATUS_FAILURE, "");

    *msg = STEPMOTOR_CMD_SET_SPEED;
    memcpy(msg + 1, &speed, sizeof(uint32_t));

    posix_manager_send(&p_motor->key, msg, sizeof(uint8_t) + sizeof(uint32_t));
    posix_manager_recv(&p_motor->key, &res , sizeof(uint8_t));

    return (vdev_status_t)res;
}

vdev_status_t posix_vdev_stepmotor_set_angle(
        _IN_ uint32_t id,
        _IN_ float angle)
{
    stepmotor_t *p_motor = NULL;
    uint8_t msg[sizeof(uint8_t) + sizeof(float)];
    uint8_t res;

    HASH_FIND_INT(pHead, &id, p_motor);
    VDEV_RETURN_IF_NULL(p_motor, VDEV_STATUS_FAILURE, "");

    *msg = STEPMOTOR_CMD_SET_ANGLE;
    memcpy(msg + 1, &angle, sizeof(float));

    posix_manager_send(&p_motor->key, msg, sizeof(uint8_t) + sizeof(float));
    posix_manager_recv(&p_motor->key, &res , sizeof(uint8_t));

    return (vdev_status_t)res;
}

vdev_status_t posix_vdev_stepmotor_step(
        _IN_ uint32_t id)
{
    stepmotor_t *p_motor = NULL;
    uint8_t cmd = STEPMOTOR_CMD_STEP;
    uint32_t res;

    HASH_FIND_INT(pHead, &id, p_motor);
    VDEV_RETURN_IF_NULL(p_motor, VDEV_STATUS_FAILURE, "");

    posix_manager_send(&p_motor->key, &cmd, sizeof(uint8_t));
    posix_manager_recv(&p_motor->key, &res , sizeof(uint8_t));

    return res;
}

void
vdev_stepmotor_api_install(vdev_stepmotor_api_t *api)
{
    api->init   = posix_vdev_stepmotor_init;
    api->step   = posix_vdev_stepmotor_step;
    api->set_speed = posix_vdev_stepmotor_set_speed;
    api->set_angle = posix_vdev_stepmotor_set_angle;
}


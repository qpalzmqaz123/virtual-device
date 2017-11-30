#include "vdev.h"
#include "vdev_model.h"
#include "posix_manager.h"
#include "uthash/include/uthash.h"


typedef enum _stepmotor_cmd_t {
    STEPMOTOR_CMD_INIT      = 0,
    STEPMOTOR_CMD_SET_SPEED = 1,
    STEPMOTOR_CMD_SET_ANGLE = 2,
    STEPMOTOR_CMD_SET_DIR   = 3,
    STEPMOTOR_CMD_STEP      = 4
} stepmotor_cmd_t;

typedef struct _stepmotor_t {
    int                 id;
    posix_manager_key_t key;
    vdev_stepmotor_t    options;
    UT_hash_handle      hh;
} stepmotor_t;


static stepmotor_t *pHead = NULL;


vdev_status_t posix_vdev_stepmotor_init(
        _IN_ uint32_t id,
        _IN_ vdev_stepmotor_t *options)
{
    stepmotor_t *p_motor = NULL;
    uint8_t msg[6];
    uint8_t res;

    VDEV_RETURN_IF_NULL(p_motor = (stepmotor_t *)malloc(sizeof(stepmotor_t)), VDEV_STATUS_OUT_OF_MEMORY, "");
    memset(p_motor, 0, sizeof(stepmotor_t));

    memcpy(&p_motor->options, options, sizeof(vdev_stepmotor_t));

    p_motor->id = id;
    p_motor->key.id = id;
    p_motor->key.model = VDEV_MODEL_STEPMOTOR;
    HASH_ADD_INT(pHead, id, p_motor);

    posix_manager_register(&p_motor->key);

    *msg = STEPMOTOR_CMD_INIT;
    *(msg + 1) = p_motor->options.dir;
    memcpy(msg + 2, &p_motor->options.speed, 4);

    posix_manager_send(&p_motor->key, &msg, sizeof(msg));
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

    if (VDEV_STATUS_SUCCESS == res) {
      p_motor->options.speed = speed;
    }

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

vdev_status_t posix_vdev_stepmotor_set_dir(
        _IN_ uint32_t id,
        _IN_ vdev_stepmotor_dir_t dir)
{
    stepmotor_t *p_motor = NULL;
    uint8_t msg[2];
    uint8_t res;

    HASH_FIND_INT(pHead, &id, p_motor);
    VDEV_RETURN_IF_NULL(p_motor, VDEV_STATUS_FAILURE, "");

    *msg = STEPMOTOR_CMD_SET_DIR;
    *(msg + 1) = (uint8_t)dir;

    posix_manager_send(&p_motor->key, msg, sizeof(msg));
    posix_manager_recv(&p_motor->key, &res , sizeof(uint8_t));

    if (VDEV_STATUS_SUCCESS == res) {
      p_motor->options.dir = dir;
    }

    return (vdev_status_t)res;
}

vdev_status_t posix_vdev_stepmotor_step(
        _IN_ uint32_t id,
        _IN_ uint32_t count)
{
    stepmotor_t *p_motor = NULL;
    uint8_t msg[5];
    uint8_t res;

    HASH_FIND_INT(pHead, &id, p_motor);
    VDEV_RETURN_IF_NULL(p_motor, VDEV_STATUS_FAILURE, "");

    *msg = STEPMOTOR_CMD_STEP;
    memcpy(msg + 1, &count, 4);

    posix_manager_send(&p_motor->key, msg, sizeof(msg));
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
    api->set_dir   = posix_vdev_stepmotor_set_dir;
}


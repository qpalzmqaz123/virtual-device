#include "vdev.h"
#include "vdev_model.h"
#include "posix_manager.h"
#include "uthash.h"


typedef enum _led_cmd_t {
    LED_CMD_INIT   = 0,
    LED_CMD_ON     = 1,
    LED_CMD_OFF    = 2,
    LED_CMD_TOGGLE = 3
} led_cmd_t;

typedef struct _led_t {
    int                 id;
    posix_manager_key_t key;
    UT_hash_handle      hh;
} led_t;


static led_t *pHead = NULL;


vdev_status_t posix_vdev_led_init(
        _IN_ uint32_t id)
{
    led_t *p_led = NULL;
    uint8_t cmd = LED_CMD_INIT;
    uint32_t res;

    VDEV_RETURN_IF_NULL(p_led = (led_t *)malloc(sizeof(led_t)), VDEV_STATUS_OUT_OF_MEMORY, "");
    memset(p_led, 0, sizeof(led_t));

    p_led->id = id;
    p_led->key.id = id;
    p_led->key.model = VDEV_MODEL_LED;
    HASH_ADD_INT(pHead, id, p_led);

    posix_manager_register(&p_led->key);

    posix_manager_send(&p_led->key, &cmd, sizeof(uint8_t));
    posix_manager_recv(&p_led->key, &res, sizeof(uint32_t));

    return res;
}

vdev_status_t posix_vdev_led_on(
        _IN_ uint32_t id)
{
    led_t *p_led = NULL;
    uint8_t cmd = LED_CMD_ON;
    uint32_t res;

    HASH_FIND_INT(pHead, &id, p_led);
    VDEV_RETURN_IF_NULL(p_led, VDEV_STATUS_FAILURE, "");

    posix_manager_send(&p_led->key, &cmd, sizeof(uint8_t));
    posix_manager_recv(&p_led->key, &res, sizeof(uint32_t));

    return res;
}

vdev_status_t posix_vdev_led_off(
        _IN_ uint32_t id)
{
    led_t *p_led = NULL;
    uint8_t cmd = LED_CMD_OFF;
    uint32_t res;

    HASH_FIND_INT(pHead, &id, p_led);
    VDEV_RETURN_IF_NULL(p_led, VDEV_STATUS_FAILURE, "");

    posix_manager_send(&p_led->key, &cmd, sizeof(uint8_t));
    posix_manager_recv(&p_led->key, &res, sizeof(uint32_t));

    return res;
}

vdev_status_t posix_vdev_led_toggle(
        _IN_ uint32_t id)
{
    led_t *p_led = NULL;
    uint8_t cmd = LED_CMD_TOGGLE;
    uint32_t res;

    HASH_FIND_INT(pHead, &id, p_led);
    VDEV_RETURN_IF_NULL(p_led, VDEV_STATUS_FAILURE, "");

    posix_manager_send(&p_led->key, &cmd, sizeof(uint8_t));
    posix_manager_recv(&p_led->key, &res, sizeof(uint32_t));

    return res;
}

void
vdev_led_api_install(vdev_led_api_t *api)
{
    api->init   = posix_vdev_led_init;
    api->on     = posix_vdev_led_on;
    api->off    = posix_vdev_led_off;
    api->toggle = posix_vdev_led_toggle;

}


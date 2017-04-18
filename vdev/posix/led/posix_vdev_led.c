#include "vdev.h"
#include "vdev_model.h"
#include "posix_manager.h"
#include "uthash.h"


typedef enum _led_cmd {
    LED_CMD_INIT   = 0,
    LED_CMD_ON     = 1,
    LED_CMD_OFF    = 2,
    LED_CMD_TOGGLE = 3
} led_cmd;

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
    led_cmd cmd = LED_CMD_INIT;
    uint8_t state;

    VDEV_RETURN_IF_NULL(p_led = (led_t *)malloc(sizeof(led_t)), VDEV_STATUS_NO_MEMORY, "");
    memset(p_led, 0, sizeof(led_t));

    p_led->id = id;
    p_led->key.id = id;
    p_led->key.model = VDEV_MODEL_LED;
    HASH_ADD_INT(pHead, id, p_led);

    posix_manager_register(&p_led->key);

    /* wait init done */
    posix_manager_send(&p_led->key, &cmd, 1);
    posix_manager_recv(&p_led->key, &state, 1);

    return VDEV_STATUS_SUCCESS;
}

vdev_status_t posix_vdev_led_on(
        _IN_ uint32_t id)
{
    led_t *p_led = NULL;
    led_cmd cmd = LED_CMD_ON;

    HASH_FIND_INT(pHead, &id, p_led);
    VDEV_RETURN_IF_NULL(p_led, VDEV_STATUS_FAILURE, "");

    posix_manager_send(&p_led->key, &cmd, 1);


    return VDEV_STATUS_SUCCESS;
}

vdev_status_t posix_vdev_led_off(
        _IN_ uint32_t id)
{
    led_t *p_led = NULL;
    led_cmd cmd = LED_CMD_OFF;

    HASH_FIND_INT(pHead, &id, p_led);
    VDEV_RETURN_IF_NULL(p_led, VDEV_STATUS_FAILURE, "");

    posix_manager_send(&p_led->key, &cmd, 1);

    return VDEV_STATUS_SUCCESS;
}

vdev_status_t posix_vdev_led_toggle(
        _IN_ uint32_t id)
{
    led_t *p_led = NULL;
    led_cmd cmd = LED_CMD_TOGGLE;

    HASH_FIND_INT(pHead, &id, p_led);
    VDEV_RETURN_IF_NULL(p_led, VDEV_STATUS_FAILURE, "");

    posix_manager_send(&p_led->key, &cmd, sizeof(uint8_t));

    return VDEV_STATUS_SUCCESS;
}

void vdev_led_api_install(vdev_led_api_t *api)
{
    api->init   = posix_vdev_led_init;
    api->on     = posix_vdev_led_on;
    api->off    = posix_vdev_led_off;
    api->toggle = posix_vdev_led_toggle;

}


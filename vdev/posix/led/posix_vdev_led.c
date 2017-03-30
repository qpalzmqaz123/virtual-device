#include "vdev.h"
#include "vdev_model.h"
#include "posix_manager.h"

typedef enum _led_cmd {
    LED_CMD_INIT   = 0,
    LED_CMD_ON     = 1,
    LED_CMD_OFF    = 2,
    LED_CMD_TOGGLE = 3
} led_cmd;

static posix_manager_key_t LedKey[2] =  {
    {VDEV_MODEL_LED, 0},
    {VDEV_MODEL_LED, 1},
};

vdev_status_t posix_vdev_led_init(
        _IN_ uint32_t id)
{
    led_cmd cmd = LED_CMD_INIT;
    uint8_t state;

    posix_manager_register(&LedKey[id]);
    VDEV_RETURN_IF_NULL(&LedKey[id], VDEV_STATUS_FAILURE, "");

    /* wait init done */
    posix_manager_send(&LedKey[id], &cmd, sizeof(uint8_t));
    posix_manager_recv(&LedKey[id], &state, sizeof(uint8_t));

    return VDEV_STATUS_SUCCESS;
}

vdev_status_t posix_vdev_led_on(
        _IN_ uint32_t id)
{
    led_cmd cmd = LED_CMD_ON;

    posix_manager_send(&LedKey[id], &cmd, sizeof(uint8_t));

    return VDEV_STATUS_SUCCESS;
}

vdev_status_t posix_vdev_led_off(
        _IN_ uint32_t id)
{
    led_cmd cmd = LED_CMD_OFF;

    posix_manager_send(&LedKey[id], &cmd, sizeof(uint8_t));

    return VDEV_STATUS_SUCCESS;
}

vdev_status_t posix_vdev_led_toggle(
        _IN_ uint32_t id)
{
    led_cmd cmd = LED_CMD_TOGGLE;

    posix_manager_send(&LedKey[id], &cmd, sizeof(uint8_t));

    return VDEV_STATUS_SUCCESS;
}

void vdev_led_api_install(vdev_led_api_t *api)
{
    api->init   = posix_vdev_led_init;
    api->on     = posix_vdev_led_on;
    api->off    = posix_vdev_led_off;
    api->toggle = posix_vdev_led_toggle;

}


#include "vdev.h"
#include "vdev_model.h"
#include "posix_manager.h"

typedef enum _led_cmd {
    LED_CMD_INIT   = 0,
    LED_CMD_ON     = 1,
    LED_CMD_OFF    = 2,
    LED_CMD_TOGGLE = 3
} led_cmd;

static posix_manager_model_t *pLedModel = NULL;

vdev_status_t posix_vdev_led_init(
        _IN_ uint32_t id)
{
    led_cmd cmd = LED_CMD_INIT;

    pLedModel = posix_manager_register(VDEV_MODEL_LED, 0);
    VDEV_RETURN_IF_NULL(pLedModel, VDEV_STATUS_FAILURE, "");
    posix_manager_send(pLedModel, &cmd, sizeof(uint8_t));

    return VDEV_STATUS_SUCCESS;
}

vdev_status_t posix_vdev_led_on(
        _IN_ uint32_t id)
{
    led_cmd cmd = LED_CMD_ON;

    posix_manager_send(pLedModel, &cmd, sizeof(uint8_t));

    return VDEV_STATUS_SUCCESS;
}

vdev_status_t posix_vdev_led_off(
        _IN_ uint32_t id)
{
    led_cmd cmd = LED_CMD_OFF;

    posix_manager_send(pLedModel, &cmd, sizeof(uint8_t));

    return VDEV_STATUS_SUCCESS;
}

vdev_status_t posix_vdev_led_toggle(
        _IN_ uint32_t id)
{
    led_cmd cmd = LED_CMD_TOGGLE;

    posix_manager_send(pLedModel, &cmd, sizeof(uint8_t));

    return VDEV_STATUS_SUCCESS;
}

void vdev_led_api_install(vdev_led_api_t *api)
{
    api->init   = posix_vdev_led_init;
    api->on     = posix_vdev_led_on;
    api->off    = posix_vdev_led_off;
    api->toggle = posix_vdev_led_toggle;

}


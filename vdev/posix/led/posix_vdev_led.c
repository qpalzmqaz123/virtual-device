#include "vdev.h"

vdev_status_t posix_vdev_led_init(
        _IN_ uint32_t id)
{
    return VDEV_STATUS_SUCCESS;
}

vdev_status_t posix_vdev_led_on(
        _IN_ uint32_t id)
{
    return VDEV_STATUS_SUCCESS;
}

vdev_status_t posix_vdev_led_off(
        _IN_ uint32_t id)
{
    return VDEV_STATUS_SUCCESS;
}

vdev_status_t posix_vdev_led_toggle(
        _IN_ uint32_t id)
{
    return VDEV_STATUS_SUCCESS;
}

void vdev_led_api_install(vdev_led_api_t *api)
{
    api->init   = posix_vdev_led_init;
    api->on     = posix_vdev_led_on;
    api->off    = posix_vdev_led_off;
    api->toggle = posix_vdev_led_toggle;

}


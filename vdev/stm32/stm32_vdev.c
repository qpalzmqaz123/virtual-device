#include <stdlib.h>
#include <string.h>
#include "vdev.h"
#include "stm32_vdev_led.h"
#include "stm32_vdev_os.h"
#include "stm32_vdev_lcd.h"

static vdev_api_t VdevApi;

static void stm32_install_all_api(vdev_api_t *api)
{
#if VDEV_SUPPORT_LCD
    vdev_lcd_api_install(&api->lcd);
#endif
#if VDEV_SUPPORT_OS
    vdev_os_api_install(&api->os);
#endif
#if VDEV_SUPPORT_LED
    vdev_led_api_install(&api->led);
#endif
}

vdev_status_t vdev_api_init(void)
{
    memset(&VdevApi, 0, sizeof(vdev_api_t));

    /* initial api */
    stm32_install_all_api(&VdevApi);

    return VDEV_STATUS_SUCCESS;
}

vdev_status_t vdev_api_destroy(void)
{
    return VDEV_STATUS_SUCCESS;
}

vdev_api_t *vdev_get_api(void)
{
    return &VdevApi;
}

#if VDEV_SUPPORT_LOG
vdev_status_t vdev_set_log(
        _IN_ vdev_log_t log)
{
    if (NULL == pVdevApi) {
        return VDEV_STATUS_FAILURE;
    }
    pVdevApi->log = log;

    return VDEV_STATUS_SUCCESS;
}
#endif


#include <stdlib.h>
#include <string.h>
#include "vdev.h"
#include "posix_manager.h"
#include "posix_vdev_lcd.h"
#include "posix_vdev_os.h"
#include "posix_vdev_led.h"

static vdev_api_t *pVdevApi = NULL;

static void posix_install_all_api(vdev_api_t *api)
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
    pVdevApi = (vdev_api_t *)malloc(sizeof(vdev_api_t));
    if (NULL == pVdevApi) {
        return VDEV_STATUS_NO_MEMORY;
    }
    memset(pVdevApi, 0, sizeof(vdev_api_t));

    /* initial api */
    posix_install_all_api(pVdevApi);

    /* initial manager */
    posix_manager_init();

    return VDEV_STATUS_SUCCESS;
}

vdev_status_t vdev_api_destroy(void)
{
#if VDEV_SUPPORT_OS
    pVdevApi->os.destroy();
#endif
    free(pVdevApi);

    return VDEV_STATUS_SUCCESS;
}

vdev_api_t *vdev_get_api(void)
{
    return pVdevApi;
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


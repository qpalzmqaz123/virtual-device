#include <stdlib.h>
#include <string.h>
#include "vdev.h"
#include "posix_manager.h"
#include "posix_vdev_lcd.h"
#include "posix_vdev_os.h"
#include "posix_vdev_led.h"


static void *pApis[VDEV_API_MAX] = {NULL}; 


static vdev_status_t
posix_install_all_api(vdev_api_t *p_api, uint32_t count)
{
    uint32_t i;
    uint32_t id;
    vdev_api_t *p;

    for (i = 0, p = p_api; i < count; i++, p++) {

        id = p->id;

        switch (p->model) {
            case VDEV_MODEL_GPIO:
                break;
            case VDEV_MODEL_LCD:
                pApis[id] = malloc(sizeof(vdev_lcd_api_t));
                vdev_lcd_api_install(pApis[id]);
                break;
            case VDEV_MODEL_OS:
                pApis[id] = malloc(sizeof(vdev_os_api_t));
                vdev_os_api_install(pApis[id]);
                break;
            case VDEV_MODEL_LED:
                pApis[id] = malloc(sizeof(vdev_led_api_t));
                vdev_led_api_install(pApis[id]);
                break;
            case VDEV_MODEL_SDCARD:
                break;
            default:
                return VDEV_STATUS_NOT_EXIST;
        }
    }

    return VDEV_STATUS_SUCCESS;
}

vdev_status_t vdev_api_init(
        _IN_ vdev_api_t *p_api,
        _IN_ uint32_t count)
{

    /* initial manager */
    posix_manager_init();

    /* initial api */
    return posix_install_all_api(p_api, count);
}

void *vdev_api_get(
        _IN_ vdev_api_id_t id)
{
    if (id < VDEV_API_MAX) {
        return pApis[id];
    }
    else {
        return NULL;
    }
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


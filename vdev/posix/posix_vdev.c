#include <stdlib.h>
#include <string.h>
#include "vdev.h"
#include "posix_manager.h"
#include "posix_vdev_lcd.h"
#include "posix_vdev_os.h"
#include "posix_vdev_led.h"
#include "posix_vdev_sdcard.h"
#include "posix_vdev_stepmotor.h"


typedef struct _install_tbl_t {
    vdev_api_id_t  id;
    size_t         size;
    void (*fn)(void *);
} install_tbl_t;


static void *pApis[VDEV_API_MAX] = {NULL}; 
static const install_tbl_t InstTbl[] = {
    {VDEV_API_LED,       sizeof(vdev_led_api_t),       (void (*)(void *))vdev_led_api_install},
    {VDEV_API_LCD,       sizeof(vdev_lcd_api_t),       (void (*)(void *))vdev_lcd_api_install},
    {VDEV_API_OS,        sizeof(vdev_os_api_t),        (void (*)(void *))vdev_os_api_install},
    {VDEV_API_SDCARD,    sizeof(vdev_sdcard_api_t),    (void (*)(void *))vdev_sdcard_api_install},
    {VDEV_API_STEPMOTOR, sizeof(vdev_stepmotor_api_t), (void (*)(void *))vdev_stepmotor_api_install}
};


static vdev_status_t
posix_install_all_api(vdev_api_t *p_api, size_t count)
{
    int i, j;
    int id;

    for (i = 0; i < count; i++) {
        id = p_api[i].id;

        for (j = 0; j < sizeof(InstTbl) / sizeof(InstTbl[0]); j++) {
            if (id == InstTbl[j].id) {
                pApis[id] = malloc(InstTbl[j].size);
                InstTbl[j].fn(pApis[id]);
            }
        }
    }

    return VDEV_STATUS_SUCCESS;
}

vdev_status_t vdev_api_init(
        _IN_ vdev_api_t *p_api,
        _IN_ size_t count)
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


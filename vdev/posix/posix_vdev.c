#include <stdlib.h>
#include <string.h>
#include "vdev.h"
#include "posix_manager.h"
#include "posix_vdev_lcd.h"
#include "posix_vdev_os.h"
#include "posix_vdev_led.h"
#include "posix_vdev_sdcard.h"
#include "posix_vdev_stepmotor.h"
#include "posix_vdev_flowmeter.h"


typedef struct _install_tbl_t {
    vdev_model_t   model;
    size_t         size;
    void (*fn)(void *);
} install_tbl_t;


static const install_tbl_t InstTbl[] = {
    {VDEV_MODEL_LED,       sizeof(vdev_led_api_t),       (void (*)(void *))vdev_led_api_install},
    {VDEV_MODEL_LCD,       sizeof(vdev_lcd_api_t),       (void (*)(void *))vdev_lcd_api_install},
    {VDEV_MODEL_OS,        sizeof(vdev_os_api_t),        (void (*)(void *))vdev_os_api_install},
    {VDEV_MODEL_SDCARD,    sizeof(vdev_sdcard_api_t),    (void (*)(void *))vdev_sdcard_api_install},
    {VDEV_MODEL_STEPMOTOR, sizeof(vdev_stepmotor_api_t), (void (*)(void *))vdev_stepmotor_api_install},
    {VDEV_MODEL_FLOWMETER, sizeof(vdev_flowmeter_api_t), (void (*)(void *))vdev_flowmeter_api_install}
};


static vdev_status_t
posix_install_all_api(vdev_api_list_entry_t *p_entry)
{
    int i, j;
    vdev_model_t model;
    const char *mount_point;
    void *p_api;

    for (i = 0; VDEV_MODEL_NULL != p_entry[i].model; i++) {
        model = p_entry[i].model;
        mount_point = p_entry[i].mount_point;

        for (j = 0; j < sizeof(InstTbl) / sizeof(InstTbl[0]); j++) {
            if (model == InstTbl[j].model) {
                p_api = malloc(InstTbl[j].size);

                /* install */
                InstTbl[j].fn(p_api);

                /* register */
                vdev_api_register(p_api, mount_point);
            }
        }
    }

    return VDEV_STATUS_SUCCESS;
}

vdev_status_t vdev_api_init(_IN_ vdev_api_list_entry_t *p_api)
{
    /* initial manager */
    posix_manager_init();

    /* initial api */
    return posix_install_all_api(p_api);
}

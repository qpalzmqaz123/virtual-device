#include <stdlib.h>
#include "vdev.h"
#include "posix_vdev_lcd.h"

static vdev_api_t *pVdevApi = NULL;

static void posix_install_all_api(vdev_api_t *api)
{
#if VDEV_SUPPORT_LCD == 1
    vdev_lcd_api_install(&api->lcd);
#endif
}

vdev_status_t vdev_api_init(void)
{
    pVdevApi = (vdev_api_t *)malloc(sizeof(vdev_api_t));
    VDEV_ASSERT_NOT_NULL(pVdevApi);

    posix_install_all_api(pVdevApi);
    return VDEV_STATUS_SUCCESS;
}

vdev_api_t *vdev_get_api(void)
{
    return pVdevApi;
}


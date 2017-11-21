#if 1
#include <stdio.h>
#include <string.h>
#include "vdev.h"
#include "GUI.h"
#include "DIALOG.h"


static vdev_api_list_entry_t Apis[] = {
    { VDEV_MODEL_LED, "led" },
    { VDEV_MODEL_OS,  "os" },
    { VDEV_MODEL_LCD, "lcd" },
};


static void
bsp_init(void)
{
    vdev_os_api_t *p_os  = NULL;
    vdev_led_api_t *p_led = NULL;

    vdev_api_init(Apis);
    p_os = (vdev_os_api_t *)vdev_api_get("os");
    p_led = (vdev_led_api_t *)vdev_api_get("led");

    p_os->init();
    p_led->init(0);
    GUI_Init();
}

static void
task_touch(void *arg)
{
    vdev_os_api_t *p_os  = NULL;

    p_os = (vdev_os_api_t *)vdev_api_get("os");

    while (1) {
        p_os->msleep(10);
        GUI_LOCK();
        GUI_TOUCH_Exec();
        GUI_UNLOCK();
    }
}

static void
task_led(void *arg)
{
    vdev_os_api_t *p_os  = NULL;
    vdev_led_api_t *p_led = NULL;

    p_os = (vdev_os_api_t *)vdev_api_get("os");
    p_led = (vdev_led_api_t *)vdev_api_get("led");

    while (1) {
        p_os->msleep(500);
        p_led->toggle(0); 
    }
}

extern GUI_CONST_STORAGE GUI_FONT GUI_Fontzh_CN;
static void
task_main(void *arg)
{
    vdev_os_api_t *p_os  = NULL;

    p_os = (vdev_os_api_t *)vdev_api_get("os");

    GUI_UC_SetEncodeUTF8();
    GUI_SetFont(&GUI_Fontzh_CN);
    GUI_DispString("中文测试");
    BUTTON_SetDefaultFont(&GUI_Fontzh_CN);
    while (1) {
        GUI_Exec(); 
        p_os->msleep(1);
    }
}

int
main(void)
{
    vdev_os_api_t *p_os  = NULL;
    vdev_os_task_t led, task1, task2;

    bsp_init();

    p_os = (vdev_os_api_t *)vdev_api_get("os");

    p_os->task_create(&led,   task_led, (void *)NULL, "led");
    p_os->task_create(&task1, task_main, (void *)NULL, "main");
    p_os->task_create(&task2, task_touch, (void *)NULL, "touch");

    p_os->task_start();
}

#else

#include <stdio.h>
#include <string.h>
#include "vdev.h"

static vdev_api_list_entry_t Apis[] = {
    { VDEV_MODEL_LED, "led" },
    { VDEV_MODEL_OS,  "os"  },
    { VDEV_MODEL_NULL, NULL }
};

static void
bsp_init(void)
{
    vdev_os_api_t *p_os  = NULL;
    vdev_led_api_t *p_led = NULL;

    vdev_api_init(Apis);
    p_os = (vdev_os_api_t *)vdev_api_get("os");
    p_led = (vdev_led_api_t *)vdev_api_get("led");
    printf("p_os: %x\n", p_os);

    p_os->init();
    p_led->init(0);
}

static void
task_led(void *arg)
{
    vdev_os_api_t *p_os  = NULL;
    vdev_led_api_t *p_led = NULL;

    p_os = (vdev_os_api_t *)vdev_api_get("os");
    p_led = (vdev_led_api_t *)vdev_api_get("led");

    while (1) {
        p_os->msleep(100);
        p_led->toggle(0); 
    }
}

int
main(void)
{
    vdev_os_api_t *p_os  = NULL;
    vdev_os_task_t led, task1, task2;

    bsp_init();

    p_os = (vdev_os_api_t *)vdev_api_get("os");

    p_os->task_create(&led,   task_led, (void *)NULL, "led");

    p_os->task_start();
}
#endif

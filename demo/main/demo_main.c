#include <stdio.h>
#include <string.h>
#include "vdev.h"
#include "GUI.h"
#include "DIALOG.h"


static vdev_api_t Apis[] = {
    {VDEV_API_LED,       VDEV_MODEL_LED},
    {VDEV_API_OS,        VDEV_MODEL_OS},
    {VDEV_API_LCD,       VDEV_MODEL_LCD},
};


static void
bsp_init(void)
{
    vdev_os_api_t *p_os  = NULL;
    vdev_led_api_t *p_led = NULL;

    vdev_api_init(Apis, sizeof(Apis) / sizeof(vdev_api_t));
    p_os = (vdev_os_api_t *)vdev_api_get(VDEV_API_OS);
    p_led = (vdev_led_api_t *)vdev_api_get(VDEV_API_LED);

    p_os->init();
    p_led->init(0);
    GUI_Init();
}

static void
task_touch(void *arg)
{
    vdev_os_api_t *p_os  = NULL;

    p_os = (vdev_os_api_t *)vdev_api_get(VDEV_API_OS);

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

    p_os = (vdev_os_api_t *)vdev_api_get(VDEV_API_OS);
    p_led = (vdev_led_api_t *)vdev_api_get(VDEV_API_LED);

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

    p_os = (vdev_os_api_t *)vdev_api_get(VDEV_API_OS);

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

    p_os = (vdev_os_api_t *)vdev_api_get(VDEV_API_OS);

    p_os->task_create(&led,   task_led, (void *)NULL, "led");
    p_os->task_create(&task1, task_main, (void *)NULL, "main");
    p_os->task_create(&task2, task_touch, (void *)NULL, "touch");

    p_os->task_start();
}


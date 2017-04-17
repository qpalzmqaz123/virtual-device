#include <stdio.h>
#include <string.h>
#include "vdev.h"
#include "GUI.h"
#include "DIALOG.h"

#if 0
static vdev_api_t Apis[] = {
    {VDEV_API_LED, VDEV_MODEL_LED},
    {VDEV_API_OS,  VDEV_MODEL_OS},
    {VDEV_API_LCD, VDEV_MODEL_LCD}
};


void bsp_init(void)
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

void task_touch(void *arg)
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

void task_led(void *arg)
{
    vdev_os_api_t *p_os  = NULL;
    vdev_led_api_t *p_led = NULL;

    p_os = (vdev_os_api_t *)vdev_api_get(VDEV_API_OS);
    p_led = (vdev_led_api_t *)vdev_api_get(VDEV_API_LED);

    while (1) {
        p_led->toggle(0); 
        p_os->msleep(500);
    }
}

WM_HWIN CreateFramewin(void);
void task_main(void *arg)
{
    vdev_os_api_t *p_os  = NULL;

    p_os = (vdev_os_api_t *)vdev_api_get(VDEV_API_OS);

    CreateFramewin();
    while (1) {
        GUI_Exec(); 
        p_os->msleep(1);
    }
}

int main(void)
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
#else
static vdev_api_t Apis[] = {
    {VDEV_API_LED,    VDEV_MODEL_LED},
    {VDEV_API_OS,     VDEV_MODEL_OS},
    {VDEV_API_LCD,    VDEV_MODEL_LCD},
    {VDEV_API_SDCARD, VDEV_MODEL_SDCARD}
};


void bsp_init(void)
{
    vdev_sdcard_api_t *p_sd  = NULL;

    vdev_api_init(Apis, sizeof(Apis) / sizeof(vdev_api_t));
    p_sd = (vdev_sdcard_api_t *)vdev_api_get(VDEV_API_SDCARD);

    p_sd->init(0);
}

int main(void)
{
    bsp_init();
}
#endif


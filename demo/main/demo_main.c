#include <stdio.h>
#include <string.h>
#include "vdev.h"
#include "GUI.h"
#include "DIALOG.h"

static vdev_api_t *pApi = NULL;

void bsp_init(void)
{
    vdev_api_init();
    pApi = vdev_get_api();

    pApi->os.init();
    pApi->led.init(0);
    GUI_Init();
    GUI_CURSOR_Show();
}

void task_touch(void *arg)
{
    while (1) {
        pApi->os.msleep(10);
        GUI_LOCK();
        GUI_TOUCH_Exec();
        GUI_UNLOCK();
    }
}

void task_led(void *arg)
{
    while (1) {
        pApi->led.toggle(0); 
        pApi->os.msleep(500);
    }
}

WM_HWIN CreateFramewin(void);
void task_main(void *arg)
{
    CreateFramewin();
    while (1) {
        GUI_Exec(); 
        pApi->os.msleep(1);
    }
}

int demo_main(int argc, char** argv)
{
    vdev_os_task_t led, task1, task2;

    bsp_init();

    pApi->os.task_create(&led,   task_led, (void *)NULL, "led");
    pApi->os.task_create(&task1, task_main, (void *)NULL, "main");
    pApi->os.task_create(&task2, task_touch, (void *)NULL, "touch");

    pApi->os.task_start();

#if VDEV_SIMULATION_TYPE == 0
    while (1);
#else
    printf("Press 'enter' to exit\n");
    getchar();
#endif

    return 0;
}


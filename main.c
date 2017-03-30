#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "vdev.h"
#include "GUI.h"

static vdev_api_t *pApi = NULL;

void bsp_init(void)
{
    vdev_api_init();
    pApi = vdev_get_api();

    pApi->os.init();
    pApi->led.init(0);
    GUI_Init();
}

void task_touch(void *arg)
{
    while (1) {
        pApi->os.msleep(10);
        GUI_TOUCH_Exec();
    }
}

void task_test1(void *arg)
{
    GUI_MessageBox("test1", "message box", GUI_MESSAGEBOX_CF_MOVEABLE);
}

void task_test2(void *arg)
{
    GUI_MessageBox("test2", "message box", GUI_MESSAGEBOX_CF_MOVEABLE);
}

void task_main(void *arg)
{
    GUI_MessageBox("hello world", "message box", GUI_MESSAGEBOX_CF_MOVEABLE);
}

int main(int argc, char** argv)
{
    vdev_os_task_t task1, task2, task3, task4;

    bsp_init();

    pApi->os.task_create(&task1, task_main, (void *)NULL, "main");
    pApi->os.task_create(&task2, task_touch, (void *)NULL, "touch");
    pApi->os.task_create(&task3, task_test1, (void *)NULL, "test1");
    pApi->os.task_create(&task4, task_test2, (void *)NULL, "test2");

    printf("Press 'enter' to exit\n");
    getchar();

    return 0;
}


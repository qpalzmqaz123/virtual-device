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

    GUI_Init();
}

void task_touch(void *args)
{
    while (1) {
        pApi->os.msleep(10);
        GUI_TOUCH_Exec();
    }
}

void task_main(void *args)
{
    GUI_MessageBox("hello world", "message box", GUI_MESSAGEBOX_CF_MOVEABLE);
}

int main(int argc, char** argv)
{
    vdev_os_task_t task1, task2;

    bsp_init();

    pApi->os.create_task("main", task_main, (void *)NULL, &task1);
    pApi->os.create_task("touch", task_touch, (void *)NULL, &task2);

    getchar();

    return 0;
}


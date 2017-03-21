#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "vdev.h"
#include "GUI.h"

vdev_os_signal_t signal;

int test1(void *arg)
{
    printf("wait signal\n");
    vdev_get_api()->os.wait_signal(signal);
    printf("get signal\n");
    printf("wait signal\n");
    vdev_get_api()->os.wait_signal(signal);
    printf("get signal\n");

    return 0;
}

int test2(void *arg)
{
    sleep(1);
    printf("set signal\n");
    vdev_get_api()->os.set_signal(signal);
    usleep(500000);
    vdev_get_api()->os.set_signal(signal);

    return 0;
}

void bsp_init(void)
{
    vdev_api_init();
}

void test(void)
{
    vdev_api_t *api;
    vdev_os_task_t task1;
    vdev_os_task_t task2;

    api = vdev_get_api();

    api->os.init();

    api->os.create_signal(&signal);
    api->os.create_task("task1", test1, (void *)NULL, &task1);
    api->os.create_task("task2", test2, (void *)NULL, &task2);
    sleep(5);
}

int main(int argc, char** argv)
{
    bsp_init();
    test();

    return 0;
}


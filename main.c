#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "vdev.h"
#include "GUI.h"

vdev_os_mutex_t mutex;

int test1(void *arg)
{
    vdev_get_api()->os.mutex_lock(mutex);
    printf("task1\n");
    sleep(1);
    printf("exit task1\n");
    vdev_get_api()->os.mutex_unlock(mutex);

    return 0;
}

int test2(void *arg)
{
    vdev_get_api()->os.mutex_lock(mutex);
    usleep(100000);
    printf("task2\n");
    printf("exit task2\n");
    vdev_get_api()->os.mutex_unlock(mutex);

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

    api->os.create_mutex(&mutex);
    api->os.create_task("task1", test1, (void *)NULL, &task1);
    api->os.create_task("task2", test2, (void *)NULL, &task2);
    sleep(3);
    api->os.delete_mutex(mutex);
}

int main(int argc, char** argv)
{
    bsp_init();
    test();

    return 0;
}


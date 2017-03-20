#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "vdev.h"
#include "GUI.h"

int test1(void *arg)
{
    while (1) {
        printf("task1\n");
        sleep(1);
    }
}

void bsp_init(void)
{
    vdev_api_init();
}

void test(void)
{
    vdev_api_t *api;
    vdev_os_task_t task1;

    api = vdev_get_api();

    api->os.init();
    api->os.create_task("task1", test1, (void *)NULL, &task1);
    sleep(5);
    api->os.delete_task(&task1);
}

int main(int argc, char** argv)
{
    bsp_init();
    test();
    sleep(3);
    VDEV_LOG(VDEV_LOG_INFO, "quit");

    return 0;
}


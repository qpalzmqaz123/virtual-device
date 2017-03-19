#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "vdev.h"
#include "GUI.h"

#include <pthread.h>
void *touch_exec(void *arg)
{
    while (1) {
        usleep(30000);
        GUI_TOUCH_Exec();
    }
}

void create_outch_thread(void)
{
    pthread_t thread;
    pthread_create(&thread, NULL, touch_exec, (void *)NULL);
}

void bsp_init(void)
{
    vdev_api_init();
}

int main(int argc, char** argv)
{
    bsp_init();

    GUI_Init();

    GUI_CURSOR_Show();

    create_outch_thread();

    GUI_MessageBox("hello world", "Messagebox", GUI_MESSAGEBOX_CF_MOVEABLE);

    sleep(1);

    return 0;
}



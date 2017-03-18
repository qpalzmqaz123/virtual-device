#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "vdev.h"
#include "GUI.h"

void bsp_init(void)
{
    vdev_api_init();
}

int main(int argc, char** argv)
{
    bsp_init();

    GUI_Init();

//    GUI_MessageBox("hello world", "Messagebox", GUI_MESSAGEBOX_CF_MOVEABLE);

    sleep(5);

    return 0;
}


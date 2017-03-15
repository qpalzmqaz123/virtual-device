#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "vdev.h"

#include "queue.h"

void vdev_lcd_api_install(vdev_lcd_api_t *api);

int main(int argc, char** argv)
{
    vdev_api_t api;

    memset(&api, 0, sizeof(vdev_api_t));
    vdev_lcd_api_install(&api.lcd);

    api.lcd.lcd_init(0);

    sleep(5);

    return 0;
}


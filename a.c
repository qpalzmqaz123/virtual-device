#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "vdev.h"

void vdev_lcd_api_install(vdev_lcd_api_t *api);

int main(int argc, char** argv)
{
    vdev_api_t api;
    vdev_lcd_api_t *lcd_api;
    int i;


    memset(&api, 0, sizeof(vdev_api_t));
    vdev_lcd_api_install(&api.lcd);

    lcd_api = &api.lcd;

    lcd_api->lcd_init();
    for (i = 0; i < 10; i++) {
        lcd_api->lcd_fill_rect(0, 0, 480, 320, 0xf800);
        usleep(100000);
        lcd_api->lcd_fill_rect(0, 0, 480, 320, 0x001f);
        usleep(100000);
    }

    sleep(5);

    return 0;
}


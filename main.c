#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "vdev.h"

#include "queue.h"

void vdev_lcd_api_install(vdev_lcd_api_t *api);

int main(int argc, char** argv)
{
    int i, j;
    vdev_api_t api;

    memset(&api, 0, sizeof(vdev_api_t));
    vdev_lcd_api_install(&api.lcd);

    api.lcd.lcd_init(0);

    for (i = 0; i < 200; i++) {
        for (j = 0; j < 200; j++) {
            api.lcd.lcd_draw_point(0, i, j, 0xf800);
        }
    }

    sleep(5);

    return 0;
}


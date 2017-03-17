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

    api.lcd.init(0);

    for (i = 0; i < 300; i++) {
        for (j = 0; j < 300; j++) {
            api.lcd.draw_point(0, i, j, 0xf800);
        }
    }

    sleep(3);

    return 0;
}


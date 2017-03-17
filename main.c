#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "vdev.h"

#include "queue.h"

void vdev_lcd_api_install(vdev_lcd_api_t *api);

int main(int argc, char** argv)
{
    int i = 0;
    uint16_t x, y;
    vdev_status_t rs;
    vdev_api_t api;

    memset(&api, 0, sizeof(vdev_api_t));
    vdev_lcd_api_install(&api.lcd);

    api.lcd.init(0);

    api.lcd.fill_rect(0, 0, 0, 479, 319, 0xf800);

    for (i = 0; i < 30; i++) {
        rs = api.lcd.touch_get_digital(&x, &y);
        if (VDEV_STATUS_SUCCESS == rs) {
            printf("touch down: %d - %d\n", x, y);
        }
        usleep(200000);
    }

    return 0;
}


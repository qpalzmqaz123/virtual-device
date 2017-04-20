#include "vdev.h"
#include <unistd.h>
#include <pthread.h>
#include "posix_socket.h"

#define VLCD_SOCK_FILE "/tmp/vdev_lcd_socket"


typedef struct _lcd_msg_t {
    enum {
        LCD_MSG_DRAW_POINT,
        LCD_MSG_READ_POINT,
        LCD_MSG_FILL_RECT,
        LCD_MSG_TOUCH_READ
    } type;

    uint16_t x;
    uint16_t y;
    uint16_t xe;
    uint16_t ye;
    uint32_t color;
    bool     touch_down;
} lcd_msg_t;


static int LcdConn;
static pthread_mutex_t ReadMutex = PTHREAD_MUTEX_INITIALIZER;


static vdev_status_t posix_lcd_init(
       _IN_ uint32_t id)
{
    int pid;

    pid = fork();

    if (0 == pid) {
        execl("out.posix/special_lcd/special_lcd.out", "lcd", NULL);
        exit(0);
    }

    LcdConn = posix_socket_client_init(VLCD_SOCK_FILE);

    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t posix_lcd_fill_rect(
       _IN_ uint32_t id,
       _IN_ uint16_t xs,
       _IN_ uint16_t ys,
       _IN_ uint16_t xe,
       _IN_ uint16_t ye,
       _IN_ uint32_t color)
{
    lcd_msg_t msg = {
        .type = LCD_MSG_FILL_RECT,
        .x = xs,
        .y = ys,
        .xe = xe,
        .ye = ye,
        .color = color
    };

    posix_socket_write(LcdConn, &msg, sizeof(lcd_msg_t));

    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t posix_lcd_draw_point(
       _IN_ uint32_t id,
       _IN_ uint16_t x,
       _IN_ uint16_t y,
       _IN_ uint32_t color)
{
    lcd_msg_t msg = {
        .type = LCD_MSG_DRAW_POINT,
        .x = x,
        .y = y,
        .color = color
    };

    posix_socket_write(LcdConn, &msg, sizeof(lcd_msg_t));

    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t posix_lcd_get_point(
       _IN_ uint32_t id,
       _IN_ uint16_t x,
       _IN_ uint16_t y,
       _OUT_ uint32_t *color)
{
    lcd_msg_t msg = {
        .type = LCD_MSG_READ_POINT,
        .x = x,
        .y = y,
    };

    pthread_mutex_lock(&ReadMutex);

    posix_socket_write(LcdConn, &msg, sizeof(lcd_msg_t));
    posix_socket_read(LcdConn, &msg, sizeof(lcd_msg_t));

    pthread_mutex_unlock(&ReadMutex);

    *color = msg.color;

    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t posix_lcd_touch_get_xy (
        _IN_ uint32_t id,
        _OUT_ uint16_t *x,
        _OUT_ uint16_t *y)
{
    lcd_msg_t msg = {
        .type = LCD_MSG_TOUCH_READ
    };

    pthread_mutex_lock(&ReadMutex);

    posix_socket_write(LcdConn, &msg, sizeof(lcd_msg_t));
    posix_socket_read(LcdConn, &msg, sizeof(lcd_msg_t));

    pthread_mutex_unlock(&ReadMutex);

    if (!msg.touch_down) {
        return VDEV_STATUS_FAILURE;
    }
    *x = msg.x;
    *y = msg.y;

    return VDEV_STATUS_SUCCESS;
}

void vdev_lcd_api_install(vdev_lcd_api_t *api)
{
    api->init = posix_lcd_init;
    api->fill_rect = posix_lcd_fill_rect;
    api->draw_point = posix_lcd_draw_point;
    api->get_point = posix_lcd_get_point;
    api->touch_get_xy = posix_lcd_touch_get_xy;
}


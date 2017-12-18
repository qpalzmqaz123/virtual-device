#include <stdio.h>
#include "SDL.h"
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include "vdev.h"
#include "posix_socket.h"

#define VLCD_X_SIZE 480
#define VLCD_Y_SIZE 320

#define VLCD_SOCK_FILE "/tmp/vdev_lcd_socket"

typedef struct _touch_info_t {
    bool     down;
    uint16_t x;
    uint16_t y;
} touch_info_t;

typedef struct _lcd_info_t {
    SDL_Window*     window;
    SDL_Renderer*   renderer;

    pthread_t       refresh_thread;
    pthread_mutex_t refresh_mutex;
    pthread_cond_t  refresh_cond;

    pthread_t       recv_thread;

    touch_info_t    touch_info;

    uint16_t        buffer[VLCD_Y_SIZE][VLCD_X_SIZE];
} lcd_info_t;

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


static lcd_info_t *pLcdInfo = NULL;


static void
_posix_lcd_color_to_rgb(uint32_t color, uint8_t *r, uint8_t *g, uint8_t *b)
{
    *r = color >> 16;
    *g = color >> 8;
    *b = color >> 0;
}

static void
_lcd_msg_process(lcd_msg_t *msg, int conn)
{
    uint8_t r, g, b;
    uint32_t color;
    SDL_Rect rect;
    lcd_msg_t resp;

    pthread_mutex_lock(&pLcdInfo->refresh_mutex);

    switch (msg->type) {
        case LCD_MSG_DRAW_POINT:
            _posix_lcd_color_to_rgb(msg->color, &r, &g, &b);
            SDL_SetRenderDrawColor(pLcdInfo->renderer, r, g, b, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawPoint(pLcdInfo->renderer, msg->x, msg->y);
            pthread_cond_signal(&pLcdInfo->refresh_cond);
            break;
        case LCD_MSG_READ_POINT:
            rect.x = msg->x;
            rect.y = msg->y;
            rect.w = 1;
            rect.h = 1;
            SDL_RenderReadPixels(pLcdInfo->renderer, &rect, SDL_PIXELFORMAT_RGB888, &color, sizeof(uint32_t));
            resp.color = color;
            posix_socket_write(conn, &resp, sizeof(lcd_msg_t));
            break;
        case LCD_MSG_FILL_RECT:
            _posix_lcd_color_to_rgb(msg->color, &r, &g, &b);
            rect.x = msg->x;
            rect.y = msg->y;
            rect.w = msg->xe - msg->x + 1;
            rect.h = msg->ye - msg->y + 1;
            SDL_SetRenderDrawColor(pLcdInfo->renderer, r, g, b, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(pLcdInfo->renderer, &rect);
            pthread_cond_signal(&pLcdInfo->refresh_cond);
            break;
        case LCD_MSG_TOUCH_READ:
            resp.touch_down = pLcdInfo->touch_info.down;
            resp.x = pLcdInfo->touch_info.x;
            resp.y = pLcdInfo->touch_info.y;
            posix_socket_write(conn, &resp, sizeof(lcd_msg_t));
            break;
        default:
            break;
    }

    pthread_mutex_unlock(&pLcdInfo->refresh_mutex);
}

static void *
_refresh_task(void *arg)
{
    while (1) {
        pthread_mutex_lock(&pLcdInfo->refresh_mutex);

        SDL_RenderPresent(pLcdInfo->renderer);
        pthread_cond_wait(&pLcdInfo->refresh_cond, &pLcdInfo->refresh_mutex);

        pthread_mutex_unlock(&pLcdInfo->refresh_mutex);
        usleep(20000);
    }

    return NULL;
}

static void *
_recv_task(void *arg)
{
    int srv;
    int conn;
    lcd_msg_t msg;

    srv = posix_socket_server_init(1, VLCD_SOCK_FILE);
    conn = posix_socket_accept(srv);

    while (1) {
        posix_socket_read(conn, &msg, sizeof(lcd_msg_t));
        _lcd_msg_process(&msg, conn);
    }

    return NULL;
}

static void
_sdl_lcd_run(void)
{
    bool quit = false;
    SDL_Event event;

    pLcdInfo->window = SDL_CreateWindow("LCD simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, VLCD_X_SIZE, VLCD_Y_SIZE, SDL_WINDOW_SHOWN);
    VDEV_ASSERT_NOT_NULL(pLcdInfo->window);
    pLcdInfo->renderer = SDL_CreateRenderer(pLcdInfo->window, -1, SDL_RENDERER_SOFTWARE);

    pthread_mutex_init(&pLcdInfo->refresh_mutex, NULL);
    pthread_cond_init(&pLcdInfo->refresh_cond, NULL);

    pthread_create(&pLcdInfo->refresh_thread, NULL, _refresh_task, NULL);
    pthread_create(&pLcdInfo->recv_thread, NULL, _recv_task, NULL);

    while (!quit) {
        SDL_WaitEvent(&event);
		switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                pLcdInfo->touch_info.down = true;
                pLcdInfo->touch_info.x = event.button.x;
                pLcdInfo->touch_info.y = event.button.y;
                break;
            case SDL_MOUSEBUTTONUP:
                pLcdInfo->touch_info.down = false;
                break;
            case SDL_MOUSEMOTION:
                if (pLcdInfo->touch_info.down) {
                    pLcdInfo->touch_info.x = event.motion.x;
                    pLcdInfo->touch_info.y = event.motion.y;
                }
                break;
			default:
				break;
		}
    }

    SDL_Quit();
    exit(0);
}

void sdl_lcd_run(void)
{
    pLcdInfo = (lcd_info_t *)malloc(sizeof(lcd_info_t));
    _sdl_lcd_run();
}


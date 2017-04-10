#include "vdev.h"

#if VDEV_SUPPORT_LCD == 1

#include <unistd.h>
#include <pthread.h>
#include "SDL.h"
#include "posix_socket.h"

#define VLCD_X_SIZE 480
#define VLCD_Y_SIZE 320

#define VLCD_SOCK_FILE "/tmp/vdev_lcd_socket"

typedef struct _touch_info_t {
    BOOL     down;
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
static int LcdConn;
static pthread_mutex_t ReadMutex = PTHREAD_MUTEX_INITIALIZER;


static void _posix_lcd_color16_to_24(uint16_t color, uint8_t *r, uint8_t *g, uint8_t *b)
{
    *r = (color >> 8) & 0xf8;
    *g = (color >> 3) & 0xfc;
    *b = (color << 3) & 0xf8;
}


static void *_refresh_task(void *arg)
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


static void _lcd_msg_process(lcd_msg_t *msg, int conn)
{
    uint8_t r, g, b;
    uint32_t color;
    SDL_Rect rect;
    lcd_msg_t resp;

    pthread_mutex_lock(&pLcdInfo->refresh_mutex);

    switch (msg->type) {
        case LCD_MSG_DRAW_POINT:
            _posix_lcd_color16_to_24(msg->color, &r, &g, &b);
            SDL_SetRenderDrawColor(pLcdInfo->renderer, r, g, b, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawPoint(pLcdInfo->renderer, msg->x, msg->y);
            pthread_cond_signal(&pLcdInfo->refresh_cond);
            break;
        case LCD_MSG_READ_POINT:
            rect.x = msg->x;
            rect.y = msg->y;
            rect.w = 1;
            rect.h = 1;
            SDL_RenderReadPixels(pLcdInfo->renderer, &rect, SDL_PIXELFORMAT_RGB565, &color, sizeof(uint32_t));
            resp.color = color;
            posix_socket_write(conn, &resp, sizeof(lcd_msg_t));
            break;
        case LCD_MSG_FILL_RECT:
            _posix_lcd_color16_to_24(msg->color, &r, &g, &b);
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


static void *_recv_task(void *arg)
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


static void _sdl_lcd_run(void)
{
    bool quit = FALSE;
    SDL_Event event;

#if 0
    VDEV_ASSERT_SUCCESS(SDL_Init(SDL_INIT_VIDEO));
    VDEV_ASSERT_SUCCESS(SDL_CreateWindowAndRenderer(VLCD_X_SIZE, VLCD_Y_SIZE, 0, &pLcdInfo->window, &pLcdInfo->renderer));
    SDL_SetWindowTitle(pLcdInfo->window, "LCD simulator");
#else
    pLcdInfo->window = SDL_CreateWindow("LCD simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, VLCD_X_SIZE, VLCD_Y_SIZE, SDL_WINDOW_SHOWN);
    VDEV_ASSERT_NOT_NULL(pLcdInfo->window);
    pLcdInfo->renderer = SDL_CreateRenderer(pLcdInfo->window, -1, SDL_RENDERER_SOFTWARE);
#endif

    pthread_mutex_init(&pLcdInfo->refresh_mutex, NULL);
    pthread_cond_init(&pLcdInfo->refresh_cond, NULL);

    pthread_create(&pLcdInfo->refresh_thread, NULL, _refresh_task, NULL);
    pthread_create(&pLcdInfo->recv_thread, NULL, _recv_task, NULL);

    while (!quit) {
        SDL_WaitEvent(&event);
		switch (event.type) {
            case SDL_QUIT:
                quit = TRUE;
                break;
            case SDL_MOUSEBUTTONDOWN:
                pLcdInfo->touch_info.down = TRUE;
                pLcdInfo->touch_info.x = event.button.x;
                pLcdInfo->touch_info.y = event.button.y;
                break;
            case SDL_MOUSEBUTTONUP:
                pLcdInfo->touch_info.down = FALSE;
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


static vdev_status_t posix_lcd_init(
       _IN_ uint32_t id)
{
    int pid;

    pLcdInfo = (lcd_info_t *)malloc(sizeof(lcd_info_t));
    VDEV_ASSERT_NOT_NULL(pLcdInfo);
    memset(pLcdInfo, 0, sizeof(lcd_info_t));

    pid = fork();

    if (pid != 0) {
        _sdl_lcd_run();
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
       _IN_ uint16_t color)
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
       _IN_ uint16_t color)
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
       _OUT_ uint16_t *color)
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


#if VDEV_SUPPORT_TOUCH == 1
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
#endif

void vdev_lcd_api_install(vdev_lcd_api_t *api)
{
    api->init = posix_lcd_init;
    api->fill_rect = posix_lcd_fill_rect;
    api->draw_point = posix_lcd_draw_point;
    api->get_point = posix_lcd_get_point;
#if VDEV_SUPPORT_TOUCH == 1
    api->touch_get_xy = posix_lcd_touch_get_xy;
#endif
}

#endif


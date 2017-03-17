#include "vdev.h"

#if VDEV_SUPPORT_LCD == 1

#include "SDL.h"
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#define VLCD_X_SIZE 480
#define VLCD_Y_SIZE 320

typedef struct _lcd_info_t {
    SDL_Window*     window;
    SDL_Renderer*   renderer;
    pthread_t       refresh_thread;
    BOOL            refresh_flag;
    pthread_mutex_t mutex;
    uint16_t        buffer[VLCD_Y_SIZE][VLCD_X_SIZE];
} lcd_info_t;

static lcd_info_t *pLcdInfo = NULL;

static void _posix_lcd_color16_to_24(uint16_t color, uint8_t *r, uint8_t *g, uint8_t *b)
{
    *r = (color >> 8) & 0xf8;
    *g = (color >> 3) & 0xfc;
    *b = (color << 3) & 0xf8;
}

static void *_posix_lcd_task_refresh(void *args)
{
    while (1) {
        pthread_mutex_lock(&pLcdInfo->mutex);

        if (pLcdInfo->refresh_flag) {
            SDL_RenderPresent(pLcdInfo->renderer);
            pLcdInfo->refresh_flag = FALSE;
        }

        pthread_mutex_unlock(&pLcdInfo->mutex);
        usleep(50000);
    }

    return NULL;
}

static vdev_status_t posix_lcd_init(
       _IN_ uint32_t id)
{
    int res;

    pLcdInfo = (lcd_info_t *)malloc(sizeof(lcd_info_t));
    VDEV_ASSERT_NOT_NULL(pLcdInfo);
    memset(pLcdInfo, 0, sizeof(lcd_info_t));

    VDEV_ASSERT_SUCCESS(SDL_Init(SDL_INIT_VIDEO));
    VDEV_ASSERT_SUCCESS(SDL_CreateWindowAndRenderer(VLCD_X_SIZE, VLCD_Y_SIZE, 0, &pLcdInfo->window, &pLcdInfo->renderer));

    /* create thread */
    res = pthread_create(&pLcdInfo->refresh_thread, NULL, _posix_lcd_task_refresh, (void *)NULL);
    if (0 != res) {
        return VDEV_STATUS_FAILURE;
    }

    pthread_mutex_init(&pLcdInfo->mutex, NULL);

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
    uint16_t x, y;
    uint8_t r, g, b;
    SDL_Rect rect;

    for (y = ys; y <= ye; y++) {
        for (x = xs; x < xe; x++) {
            pLcdInfo->buffer[y][x] = color;
        }
    }

    _posix_lcd_color16_to_24(color, &r, &g, &b);
    rect.x = xs;
    rect.y = ys;
    rect.w = xe - xs;
    rect.h = ye - ys;

    pthread_mutex_lock(&pLcdInfo->mutex);

    SDL_SetRenderDrawColor(pLcdInfo->renderer, r, g, b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(pLcdInfo->renderer, &rect);
    pLcdInfo->refresh_flag = TRUE;

    pthread_mutex_unlock(&pLcdInfo->mutex);

    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t posix_lcd_draw_point(
       _IN_ uint32_t id,
       _IN_ uint16_t x,
       _IN_ uint16_t y,
       _IN_ uint16_t color)
{
    uint8_t r, g, b;
    pLcdInfo->buffer[y][x] = color;

    _posix_lcd_color16_to_24(color, &r, &g, &b);
    pthread_mutex_lock(&pLcdInfo->mutex);

    SDL_SetRenderDrawColor(pLcdInfo->renderer, r, g, b, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoint(pLcdInfo->renderer, x, y);
    pLcdInfo->refresh_flag = TRUE;

    pthread_mutex_unlock(&pLcdInfo->mutex);

    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t posix_lcd_get_point(
       _IN_ uint32_t id,
       _IN_ uint16_t x,
       _IN_ uint16_t y,
       _OUT_ uint16_t *color)
{
    *color = pLcdInfo->buffer[y][x];

    return VDEV_STATUS_SUCCESS;
}

void vdev_lcd_api_install(vdev_lcd_api_t *api)
{
    api->init = posix_lcd_init;
    api->fill_rect = posix_lcd_fill_rect;
    api->draw_point = posix_lcd_draw_point;
    api->get_point = posix_lcd_get_point;
}

#endif


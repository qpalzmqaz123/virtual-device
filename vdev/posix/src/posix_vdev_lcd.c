#include "vdev.h"

#if VDEV_SUPPORT_LCD == 1

#include "SDL.h"
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#define VLCD_X_SIZE 480
#define VLCD_Y_SIZE 320

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
static pthread_t RefreshThread;
static BOOL RefreshFlag = 0;
static pthread_mutex_t Mutex = PTHREAD_MUTEX_INITIALIZER;
static uint16_t LcdBuffer[VLCD_Y_SIZE][VLCD_X_SIZE];

static void _posix_lcd_color16_to_24(uint16_t color, uint8_t *r, uint8_t *g, uint8_t *b)
{
    *r = (color >> 8) & 0xf8;
    *g = (color >> 3) & 0xfc;
    *b = (color << 3) & 0xf8;
}

static void *_posix_lcd_task_refresh(void *args)
{
    while (1) {
        pthread_mutex_lock(&Mutex);

        if (RefreshFlag) {
            SDL_RenderPresent(renderer);
            RefreshFlag = FALSE;
        }

        pthread_mutex_unlock(&Mutex);
        usleep(50000);
    }

    return NULL;
}

static vdev_status_t posix_lcd_init(
       _IN_ uint32_t id)
{
    int res;

    VDEV_ASSERT_SUCCESS(SDL_Init(SDL_INIT_VIDEO));
    VDEV_ASSERT_SUCCESS(SDL_CreateWindowAndRenderer(VLCD_X_SIZE, VLCD_Y_SIZE, 0, &window, &renderer));

    /* create thread */
    res = pthread_create(&RefreshThread, NULL, _posix_lcd_task_refresh, (void *)NULL);
    if (0 != res) {
        return VDEV_STATUS_FAILURE;
    }

    pthread_mutex_init(&Mutex, NULL);

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
            LcdBuffer[y][x] = color;
        }
    }

    _posix_lcd_color16_to_24(color, &r, &g, &b);
    rect.x = xs;
    rect.y = ys;
    rect.w = xe - xs;
    rect.h = ye - ys;

    pthread_mutex_lock(&Mutex);

    SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &rect);
    RefreshFlag = TRUE;

    pthread_mutex_unlock(&Mutex);

    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t posix_lcd_draw_point(
       _IN_ uint32_t id,
       _IN_ uint16_t x,
       _IN_ uint16_t y,
       _IN_ uint16_t color)
{
    uint8_t r, g, b;
    LcdBuffer[y][x] = color;

    _posix_lcd_color16_to_24(color, &r, &g, &b);
    pthread_mutex_lock(&Mutex);

    SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoint(renderer, x, y);
    RefreshFlag = TRUE;

    pthread_mutex_unlock(&Mutex);

    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t posix_lcd_get_point(
       _IN_ uint32_t id,
       _IN_ uint16_t x,
       _IN_ uint16_t y,
       _OUT_ uint16_t *color)
{
    *color = LcdBuffer[y][x];

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


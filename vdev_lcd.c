#include "vdev.h"

#include "SDL.h"
#include <unistd.h>
#include <pthread.h>

#define VLCD_X_SIZE 480
#define VLCD_Y_SIZE 320

static pthread_t RefreshThread;
static uint16_t LcdBuffer[VLCD_Y_SIZE][VLCD_X_SIZE] = {0};

static void _vdev_lcd_color16_to_24(uint16_t color, uint8_t *r, uint8_t *g, uint8_t *b)
{
    *r = (color >> 11) * 0xff / 0x1f;
    *g = ((color >> 5) & 0x3f) * 0xff / 0x3f;
    *b = (color & 0x1f) * 0xff / 0x1f;
}

static void _vdev_lcd_do_refresh(SDL_Renderer* renderer)
{
    uint16_t x, y;
    uint8_t r, g, b;
    uint16_t color;

    for (y = 0; y < VLCD_Y_SIZE; y++) {
        for (x = 0; x < VLCD_X_SIZE; x++) {

            color = LcdBuffer[y][x];
            _vdev_lcd_color16_to_24(color, &r, &g, &b);

            SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawPoint(renderer, x, y);
        } 
    }
    SDL_RenderPresent(renderer);
}

static void *_vdev_lcd_task_refresh(void *args)
{
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    /* init */
    if (0 != SDL_Init(SDL_INIT_VIDEO)) {
        printf("Can't init SDL\n");
        exit(1);
    }
    if (0 != SDL_CreateWindowAndRenderer(VLCD_X_SIZE, VLCD_Y_SIZE, 0, &window, &renderer)) {
        printf("Can't create window\n");
        exit(1);
    }

    /* refresh */
    while(1) {
        _vdev_lcd_do_refresh(renderer);
    }


    /* destory */
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }

    SDL_Quit();
}

static vdev_status_t posix_lcd_init(void)
{
    int res;

    /* create thread */
    res = pthread_create(&RefreshThread, NULL, _vdev_lcd_task_refresh, (void *)NULL);
    if (0 != res) {
        return VDEV_STATUS_FAILURE;
    }

    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t posix_lcd_fill_rect(
       _IN_ uint16_t xs,
       _IN_ uint16_t ys,
       _IN_ uint16_t xe,
       _IN_ uint16_t ye,
       _IN_ uint16_t color)
{
    uint16_t x, y;

    for (y = ys; y <= ye; y++) {
        for (x = xs; x < xe; x++) {
            LcdBuffer[y][x] = color;
        }
    }

    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t posix_lcd_draw_point(
        _IN_ uint16_t x,
        _IN_ uint16_t y,
        _IN_ uint16_t color)
{
    LcdBuffer[y][x] = color;

    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t posix_lcd_get_point(
        _IN_ uint16_t x,
        _IN_ uint16_t y,
        _OUT_ uint16_t *color)
{
    *color = LcdBuffer[y][x];

    return VDEV_STATUS_SUCCESS;
}

void vdev_lcd_api_install(vdev_lcd_api_t *api)
{
    api->lcd_init = posix_lcd_init;
    api->lcd_fill_rect = posix_lcd_fill_rect;
    api->lcd_draw_point = posix_lcd_draw_point;
    api->lcd_get_point = posix_lcd_get_point;
}


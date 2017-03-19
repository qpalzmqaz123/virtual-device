#include "vdev.h"

#if VDEV_SUPPORT_LCD == 1

#include "SDL.h"
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#define VLCD_X_SIZE 480
#define VLCD_Y_SIZE 320

typedef struct _touch_info_t {
    BOOL     down;
    uint16_t x;
    uint16_t y;
} touch_info_t;

typedef struct _lcd_info_t {
    /* display */
    SDL_Window*     window;
    SDL_Renderer*   renderer;
    pthread_t       refresh_thread;
    BOOL            refresh_flag;
    pthread_mutex_t refresh_mutex;
    uint16_t        buffer[VLCD_Y_SIZE][VLCD_X_SIZE];
    /* touch */
    pthread_t       touch_thread;
    touch_info_t    touch_info;
    pthread_mutex_t touch_mutex;
} lcd_info_t;

static lcd_info_t *pLcdInfo = NULL;

static void _posix_lcd_color16_to_24(uint16_t color, uint8_t *r, uint8_t *g, uint8_t *b)
{
    *r = (color >> 8) & 0xf8;
    *g = (color >> 3) & 0xfc;
    *b = (color << 3) & 0xf8;
}

static void *_posix_lcd_task_touch(void *args)
{
    BOOL quit = FALSE;
    SDL_Event event;

    while (!quit) {
		SDL_WaitEvent(&event);
        pthread_mutex_lock(&pLcdInfo->touch_mutex);
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
        pthread_mutex_unlock(&pLcdInfo->touch_mutex);
    }

    return (void *)NULL;
}

static void *_posix_lcd_task_refresh(void *args)
{
    while (1) {
        pthread_mutex_lock(&pLcdInfo->refresh_mutex);

        if (pLcdInfo->refresh_flag) {
            SDL_RenderPresent(pLcdInfo->renderer);
            pLcdInfo->refresh_flag = FALSE;
        }

        pthread_mutex_unlock(&pLcdInfo->refresh_mutex);
        usleep(50000);
    }

    return (void *)NULL;
}

static vdev_status_t posix_lcd_init(
       _IN_ uint32_t id)
{
    pLcdInfo = (lcd_info_t *)malloc(sizeof(lcd_info_t));
    VDEV_ASSERT_NOT_NULL(pLcdInfo);
    memset(pLcdInfo, 0, sizeof(lcd_info_t));

    VDEV_ASSERT_SUCCESS(SDL_Init(SDL_INIT_VIDEO));
    VDEV_ASSERT_SUCCESS(SDL_CreateWindowAndRenderer(VLCD_X_SIZE, VLCD_Y_SIZE, 0, &pLcdInfo->window, &pLcdInfo->renderer));

    /* create thread */
    VDEV_ASSERT_SUCCESS(pthread_create(&pLcdInfo->refresh_thread, NULL, _posix_lcd_task_refresh, (void *)NULL));
    VDEV_ASSERT_SUCCESS(pthread_create(&pLcdInfo->touch_thread, NULL, _posix_lcd_task_touch, (void *)NULL));

    pthread_mutex_init(&pLcdInfo->refresh_mutex, NULL);
    pthread_mutex_init(&pLcdInfo->touch_mutex, NULL);

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
    rect.w = xe - xs + 1;
    rect.h = ye - ys + 1;

    pthread_mutex_lock(&pLcdInfo->refresh_mutex);

    SDL_SetRenderDrawColor(pLcdInfo->renderer, r, g, b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(pLcdInfo->renderer, &rect);
    pLcdInfo->refresh_flag = TRUE;

    pthread_mutex_unlock(&pLcdInfo->refresh_mutex);

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
    pthread_mutex_lock(&pLcdInfo->refresh_mutex);

    SDL_SetRenderDrawColor(pLcdInfo->renderer, r, g, b, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoint(pLcdInfo->renderer, x, y);
    pLcdInfo->refresh_flag = TRUE;

    pthread_mutex_unlock(&pLcdInfo->refresh_mutex);

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

#if VDEV_SUPPORT_TOUCH == 1
static vdev_status_t posix_lcd_touch_get_digital (
        _OUT_ uint16_t *x,
        _OUT_ uint16_t *y)
{
    pthread_mutex_lock(&pLcdInfo->touch_mutex);

    if (!pLcdInfo->touch_info.down) {
        pthread_mutex_unlock(&pLcdInfo->touch_mutex);
        return VDEV_STATUS_FAILURE;
    }

    *x = pLcdInfo->touch_info.x;
    *y = pLcdInfo->touch_info.y;

    pthread_mutex_unlock(&pLcdInfo->touch_mutex);
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
    api->touch_get_digital = posix_lcd_touch_get_digital;
#endif
}

#endif


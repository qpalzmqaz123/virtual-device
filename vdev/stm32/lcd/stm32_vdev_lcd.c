#include "vdev.h"

#if VDEV_SUPPORT_LCD == 1
#include "tft.h"


static uint16_t _color_32_to_16(uint32_t color)
{
    uint16_t tmp = 0;

    tmp |= (color >> 8) & 0xf800;
    tmp |= (color >> 5) & 0x07e0;
    tmp |= (color >> 3) & 0x001f;

    return tmp;
}


static uint32_t _color_16_to_32(uint16_t color)
{
    uint32_t tmp = 0;

    tmp |= (uint32_t)(color & 0xf800) << 8;
    tmp |= (uint32_t)(color & 0x07e0) << 5;
    tmp |= (uint32_t)(color & 0x001f) << 3;

    return tmp;
}


static vdev_status_t posix_lcd_init(
       _IN_ uint32_t id)
{
    Lcd_Init();
    Touch_Init();
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
    uint32_t i = (xe - xs + 1) * (ye - ys + 1);
    uint16_t c = _color_32_to_16(color);

    Lcd_SetWindow(xs, ys, xe, ye);
    while (i--) {
        Lcd_WriteData(c);
    }

    return VDEV_STATUS_SUCCESS;
}


static vdev_status_t posix_lcd_draw_point(
       _IN_ uint32_t id,
       _IN_ uint16_t x,
       _IN_ uint16_t y,
       _IN_ uint32_t color)
{
    uint16_t c = _color_32_to_16(color);

    Lcd_DrawPoint(x, y, c);
    return VDEV_STATUS_SUCCESS;
}


static vdev_status_t posix_lcd_get_point(
       _IN_ uint32_t id,
       _IN_ uint16_t x,
       _IN_ uint16_t y,
       _OUT_ uint32_t *color)
{
    *color = _color_16_to_32(Lcd_ReadPoint(x, y));
    return VDEV_STATUS_SUCCESS;
}


#if VDEV_SUPPORT_TOUCH == 1
static vdev_status_t posix_lcd_touch_get_xy (
        _IN_ uint32_t id,
        _OUT_ uint16_t *x,
        _OUT_ uint16_t *y)
{
    if (!TOUCH_DOWN) {
        return VDEV_STATUS_FAILURE;
    }

    Touch_GetXY(x, y);
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


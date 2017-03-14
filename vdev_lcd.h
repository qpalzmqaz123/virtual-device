#ifndef __VD_LCD_H__
#define __VD_LCD_H__

#include "vdev_types.h"

/**
 * @brief Initialize Lcd
 *
 * @param[in] x_size Lcd x size
 * @param[in] y_size Lcd y size
 *
 * @return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_lcd_init_fn) (void);

/**
 * @brief Fill rect
 *
 * @param[in] xs X coordinate of start point
 * @param[in] ys Y coordinate of start point
 * @param[in] xe X coordinate of end point
 * @param[in] ye Y coordinate of end point
 * @param[in] color Color
 *
 * @return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_lcd_fill_rect_fn) (
        _IN_ uint16_t xs,
        _IN_ uint16_t ys,
        _IN_ uint16_t xe,
        _IN_ uint16_t ye,
        _IN_ uint16_t color);

/**
 * @brief Draw point
 *
 * @param[in] x X coordinate
 * @param[in] y Y coordinate 
 * @param[in] color Color
 *
 * @return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_lcd_draw_point_fn) (
        _IN_ uint16_t x,
        _IN_ uint16_t y,
        _IN_ uint16_t color);

/**
 * @brief Get point color
 *
 * @param[in] x X coordinate
 * @param[in] y Y coordinate 
 * @param[in] color Color
 *
 * @return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_lcd_get_point_fn) (
        _IN_ uint16_t x,
        _IN_ uint16_t y,
        _OUT_ uint16_t *color);

/**
 * @brief lcd apis
 */
typedef struct _vdev_lcd_api_t {
    vdev_lcd_init_fn       lcd_init;
    vdev_lcd_fill_rect_fn  lcd_fill_rect;
    vdev_lcd_draw_point_fn lcd_draw_point;
    vdev_lcd_get_point_fn  lcd_get_point;
} vdev_lcd_api_t;


#endif

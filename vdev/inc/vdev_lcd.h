#if !defined(__VDEV_LCD_H__) && VDEV_SUPPORT_LCD == 1
#define __VDEV_LCD_H__

#include "vdev_types.h"

/**
 * @brief Initialize Lcd
 *
 * @param[in] id Device id
 *
 * @return Return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_lcd_init_fn) (
        _IN_ uint32_t id);

/**
 * @brief Fill rect
 *
 * @param[in] id Device id
 * @param[in] xs X coordinate of start point
 * @param[in] ys Y coordinate of start point
 * @param[in] xe X coordinate of end point
 * @param[in] ye Y coordinate of end point
 * @param[in] color Color
 *
 * @return Return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_lcd_fill_rect_fn) (
        _IN_ uint32_t id,
        _IN_ uint16_t xs,
        _IN_ uint16_t ys,
        _IN_ uint16_t xe,
        _IN_ uint16_t ye,
        _IN_ uint16_t color);

/**
 * @brief Draw point
 *
 * @param[in] id Device id
 * @param[in] x X coordinate
 * @param[in] y Y coordinate 
 * @param[in] color Color
 *
 * @return Return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_lcd_draw_point_fn) (
        _IN_ uint32_t id,
        _IN_ uint16_t x,
        _IN_ uint16_t y,
        _IN_ uint16_t color);

/**
 * @brief Get point color
 *
 * @param[in] id Device id
 * @param[in] x X coordinate
 * @param[in] y Y coordinate 
 * @param[out] color Color
 *
 * @return Return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_lcd_get_point_fn) (
        _IN_ uint32_t id,
        _IN_ uint16_t x,
        _IN_ uint16_t y,
        _OUT_ uint16_t *color);

#if VDEV_SUPPORT_TOUCH == 1

/**
 * @brief Get touch AD value
 *
 * @param[out] x Touch x AD
 * @param[out] y Touch y AD
 *
 * @return Return VDEV_STATUS_SUCCESS if touch down,
 * else return VDEV_STATUS_FAILURE
 */
typedef vdev_status_t (*vdev_lcd_touch_get_digital) (
        _OUT_ uint16_t *x,
        _OUT_ uint16_t *y);

#endif

/**
 * @brief lcd apis
 */
typedef struct _vdev_lcd_api_t {
    vdev_lcd_init_fn       init;
    vdev_lcd_fill_rect_fn  fill_rect;
    vdev_lcd_draw_point_fn draw_point;
    vdev_lcd_get_point_fn  get_point;
#if VDEV_SUPPORT_TOUCH == 1
    vdev_lcd_touch_get_digital  touch_get_digital;
#endif
} vdev_lcd_api_t;


#endif

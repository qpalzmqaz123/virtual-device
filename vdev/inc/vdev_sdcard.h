#ifndef __VDEV_SDCARD_H__
#define __VDEV_SDCARD_H__

#include "vdev_types.h"


typedef struct _vdev_sdcard_info_t {
    uint32_t block_size;
    uint64_t capacity;
    bool     exist;
    bool     protected;
} vdev_sdcard_info_t;

/**
 * @brief Initialize sdcard
 *
 * @param[in] id Device id
 *
 * @return Return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_sdcard_init_fn) (
        _IN_ uint32_t id);

/**
 * @brief Read from sdcard
 *
 * @param[in] id Device id
 * @param[out] buffer Buffer
 * @param[in] selector Selector
 * @param[in] count Selector count
 *
 * @return Return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_sdcard_read_fn) (
        _IN_ uint32_t id,
        _OUT_ void *buffer,
        _IN_ uint32_t selector,
        _IN_ uint32_t count);

/**
 * @brief Write in sdcard
 *
 * @param[in] id Device id
 * @param[in] buffer Buffer
 * @param[in] selector Selector
 * @param[in] count Selector count
 *
 * @return Return status code, refer to vdev_status_t
 */
typedef vdev_status_t (*vdev_sdcard_write_fn) (
        _IN_ uint32_t id,
        _IN_ void *buffer,
        _IN_ uint32_t selector,
        _IN_ uint32_t count);

/**
 * @brief Get sdcard infomation
 *
 * @param[in] id Device id
 *
 * @return Return sdcard info pointer
 */
typedef vdev_sdcard_info_t *(*vdev_sdcard_get_info_fn) (
        _IN_ uint32_t id);

/**
 * @brief sdcard apis
 */
typedef struct _vdev_sdcard_api_t {
    vdev_sdcard_init_fn      init;
    vdev_sdcard_read_fn      read;
    vdev_sdcard_write_fn     write;
    vdev_sdcard_get_info_fn  get_info;
} vdev_sdcard_api_t;


#endif

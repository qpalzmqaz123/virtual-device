#include "vdev.h"
#include "posix_queue.h"


static vdev_status_t posix_vdev_os_queue_create(
        _OUT_ vdev_os_queue_t *queue,
        _IN_ uint32_t length,
        _IN_ uint32_t itemsize)
{
    posix_queue_t *q = NULL;

    q = (posix_queue_t *)malloc(sizeof(posix_queue_t));
    VDEV_RETURN_IF_NULL(q, VDEV_STATUS_OUT_OF_MEMORY, "");
    VDEV_RETURN_IF_ERROR(posix_queue_create(q, length, itemsize),
                         VDEV_STATUS_FAILURE, "");
    *queue = q;

    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t posix_vdev_os_queue_delete(
        _IN_ vdev_os_queue_t queue)
{
    posix_queue_delete(queue);
    free(queue);
    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t posix_vdev_os_queue_get(
        _IN_ vdev_os_queue_t queue,
        _OUT_ void *item)
{
    VDEV_RETURN_IF_ERROR(posix_queue_get(queue, item),
                         VDEV_STATUS_FAILURE, "");

    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t posix_vdev_os_queue_put(
        _IN_ vdev_os_queue_t queue,
        _IN_ void *item)
{
    VDEV_RETURN_IF_ERROR(posix_queue_put(queue, item),
                         VDEV_STATUS_FAILURE, "");

    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t posix_vdev_os_queue_get_timeout(
        _IN_ vdev_os_queue_t queue,
        _OUT_ void *item,
        _IN_ uint32_t ms)
{
    VDEV_RETURN_IF_ERROR(posix_queue_get_timeout(queue, item, ms),
                         VDEV_STATUS_TIMEOUT, "");
    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t posix_vdev_os_queue_put_timeout(
        _IN_ vdev_os_queue_t queue,
        _IN_ void *item,
        _IN_ uint32_t ms)
{
    VDEV_RETURN_IF_ERROR(posix_queue_put_timeout(queue, item, ms),
                         VDEV_STATUS_TIMEOUT, "");
    return VDEV_STATUS_SUCCESS;
}

void
vdev_os_queue_api_install(vdev_os_api_t *api)
{
    api->queue_create      = posix_vdev_os_queue_create;
    api->queue_delete      = posix_vdev_os_queue_delete;
    api->queue_get         = posix_vdev_os_queue_get;
    api->queue_put         = posix_vdev_os_queue_put;
    api->queue_get_timeout = posix_vdev_os_queue_get_timeout;
    api->queue_put_timeout = posix_vdev_os_queue_put_timeout;
}


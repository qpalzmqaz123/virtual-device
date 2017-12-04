#include <string.h>
#include "uthash/uthash.h"
#include "vdev.h"

typedef struct api_hash_s {
    void *p_api;
    char *mount_point;
    UT_hash_handle hh;
} api_hash_t;

static api_hash_t *g_apis = NULL;

__attribute__((weak)) vdev_status_t
vdev_api_register(_IN_ void  *p_api,
                  _IN_ const char *mount_point)
{
    api_hash_t *p_hash = (api_hash_t *)malloc(sizeof(api_hash_t));
    VDEV_ASSERT_NOT_NULL(p_hash);

    p_hash->p_api = p_api;

    p_hash->mount_point = (char *)malloc(strlen(mount_point) + 1);
    VDEV_ASSERT_NOT_NULL(p_hash->mount_point);
    strcpy(p_hash->mount_point, mount_point);

    HASH_ADD_STR(g_apis, mount_point, p_hash);

    return VDEV_STATUS_SUCCESS;
}

__attribute__((weak)) void *
vdev_api_query(_IN_ const char *mount_point)
{
    api_hash_t *p_hash = NULL;

    HASH_FIND_STR(g_apis, mount_point, p_hash);

    return p_hash ? p_hash->p_api : NULL;
}

#if VDEV_SUPPORT_LOG
__attribute__((weak)) vdev_status_t
vdev_set_log(_IN_ vdev_log_t log)
{
    if (NULL == pVdevApi) {
        return VDEV_STATUS_FAILURE;
    }
    pVdevApi->log = log;

    return VDEV_STATUS_SUCCESS;
}
#endif

#include "vdev.h"
#if VDEV_SUPPORT_OS == 1

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>


#define __POSIX_VDEV_OS_DEBUG__ 0


typedef struct _os_info_t {
    pthread_t **list;
    uint32_t  thread_cnt;
} os_info_t;


static os_info_t *pOsInfo = NULL;


#if __POSIX_VDEV_OS_DEBUG__
static void debug_os_info_inspect(void)
{
    uint32_t i = 0;
    uint32_t cnt = pOsInfo->thread_cnt;
    pthread_t **p;

    printf("thread_cnt: %d, list: { ", cnt);

    if (cnt) {
        for (p = pOsInfo->list; i < cnt; p++) {
            if (*p) i++;
            printf("%lx ", (long unsigned int)(*p));
        }
    }

    printf("}\n");
}
#endif

static vdev_status_t posix_vdev_os_init(void)
{
    pOsInfo = (os_info_t *)malloc(sizeof(os_info_t));
    VDEV_ASSERT_NOT_NULL(pOsInfo);
    memset(pOsInfo, 0, sizeof(os_info_t));

    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t posix_vdev_os_create_task(
        _IN_ const char *name,
        _IN_ int (*fn)(void *arg),
        _IN_ void *arg,
        _OUT_ vdev_os_task_t *task)
{
    uint32_t i;
    uint32_t index;
    pthread_t **plist;
    pthread_t *thread = NULL;

    memset(task, 0, sizeof(vdev_os_task_t));
    /* malloc new thread variable */
    thread = (pthread_t *)malloc(sizeof(pthread_t));

    /* save to thread list */
    if (pOsInfo->list == NULL) {
        pOsInfo->list = (pthread_t **)malloc(sizeof(pthread_t *));
        *pOsInfo->list = thread;
        index = 0;
    }
    else {
        /* search deleted thread */
        for (i = 0; i < pOsInfo->thread_cnt; i++) {
            plist = pOsInfo->list + i;
            if (NULL == *plist) {
                *plist = thread;
                break;
            }
        }

        /* list is full */
        if (i == pOsInfo->thread_cnt) {
            pOsInfo->list = (pthread_t **)realloc(pOsInfo->list, sizeof(pthread_t *) * (pOsInfo->thread_cnt + 1));
            *(pOsInfo->list + pOsInfo->thread_cnt) = thread;
        }

        index = i;
    }
    pOsInfo->thread_cnt++;

    /* create thread */
    VDEV_RETURN_IF_ERROR(pthread_create(thread, NULL, (void * (*)(void *))fn, (void *)arg),
                    VDEV_STATUS_FAILURE,
                    "Can't create thread");

    /* update task */
    task->id = index;

#if __POSIX_VDEV_OS_DEBUG__
    printf("Create task: %d\n", task->id);
    debug_os_info_inspect();
#endif

    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t posix_vdev_os_delete_task(
        _IN_ vdev_os_task_t *task)
{
    pthread_t **plist = pOsInfo->list + task->id;

    if (NULL == *plist) {
        return VDEV_STATUS_NOT_EXIST;
    }
    pthread_cancel(**plist);
    free(*plist);
    *plist = NULL;
    pOsInfo->thread_cnt--;

#if __POSIX_VDEV_OS_DEBUG__
    printf("Delete task: %d\n", task->id);
    debug_os_info_inspect();
#endif

    return VDEV_STATUS_SUCCESS;
}

void vdev_os_api_install(vdev_os_api_t *api)
{
    api->init = posix_vdev_os_init;
    api->create_task = posix_vdev_os_create_task;
    api->delete_task = posix_vdev_os_delete_task;
}

#endif

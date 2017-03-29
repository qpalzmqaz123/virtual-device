#include "vdev.h"
#if VDEV_SUPPORT_OS == 1

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include "posix_vdev_os.h"


typedef struct _os_info_t {
    uint32_t  thread_cnt;
    struct timeval ts;
} os_info_t;

typedef struct _os_event_t {
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
} os_event_t;


static os_info_t *pOsInfo = NULL;


static vdev_status_t posix_vdev_os_init(void)
{
    pOsInfo = (os_info_t *)malloc(sizeof(os_info_t));
    VDEV_ASSERT_NOT_NULL(pOsInfo);
    memset(pOsInfo, 0, sizeof(os_info_t));

    /* get current time */
    gettimeofday(&pOsInfo->ts, NULL);

    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t posix_vdev_os_destroy(void)
{
    free(pOsInfo);

    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t posix_vdev_os_task_create(
        _OUT_ vdev_os_task_t *task,
        _IN_ void (*fn)(void *arg),
        _IN_ void *arg,
        _IN_ const char *name)
{
    pthread_t *thread = NULL;

    /* malloc new thread variable */
    thread = (pthread_t *)malloc(sizeof(pthread_t));
    VDEV_RETURN_IF_NULL(thread, VDEV_STATUS_FAILURE, "Can't create thread");
    memset(thread, 0, sizeof(pthread_t));

    /* create thread */
    VDEV_RETURN_IF_ERROR(pthread_create(thread, NULL, (void * (*)(void *))fn, (void *)arg),
                         VDEV_STATUS_FAILURE,
                         "Can't create thread");

    pOsInfo->thread_cnt++;

    /* update task */
    *task = thread;

    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t posix_vdev_os_task_delete(
        _IN_ vdev_os_task_t task)
{
    pthread_cancel(*(pthread_t *)task);
    free(task);
    pOsInfo->thread_cnt--;

    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t posix_vdev_os_mutex_create(
        _OUT_ vdev_os_mutex_t *mutex)
{
    int res;
    pthread_mutex_t *p = NULL;

    p = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    VDEV_RETURN_IF_NULL(p, VDEV_STATUS_NO_MEMORY, "Can't create mutex");

    memset(p, 0, sizeof(pthread_mutex_t));
    res = pthread_mutex_init(p, NULL);
    if (res) {
        free(p);
        return VDEV_STATUS_FAILURE;
    }
    *mutex = p;

    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t posix_vdev_os_mutex_delete(
        _IN_ vdev_os_mutex_t mutex)
{
    pthread_mutex_destroy(mutex);
    free(mutex);
    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t posix_vdev_os_mutex_lock(
        _IN_ vdev_os_mutex_t mutex)
{
    pthread_mutex_lock(mutex);
    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t posix_vdev_os_mutex_unlock(
        _IN_ vdev_os_mutex_t mutex)
{
    pthread_mutex_unlock(mutex);
    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t posix_vdev_os_event_create(
        _OUT_ vdev_os_event_t *event)
{
    int res;
    os_event_t *p = NULL;

    p = (os_event_t *)malloc(sizeof(os_event_t));
    VDEV_RETURN_IF_NULL(p, VDEV_STATUS_NO_MEMORY, "Can't create event");

    memset(p, 0, sizeof(os_event_t));
    res = pthread_mutex_init(&p->mutex, NULL);
    res += pthread_cond_init(&p->cond, NULL);
    if (res) {
        free(p);
        return VDEV_STATUS_FAILURE;
    }
    *event = p;

    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t posix_vdev_os_event_delete(
        _IN_ vdev_os_event_t event)
{
    pthread_mutex_destroy(&((os_event_t *)event)->mutex);
    pthread_cond_destroy(&((os_event_t *)event)->cond);
    free(event);

    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t posix_vdev_os_event_set(
        _IN_ vdev_os_event_t event)
{
    pthread_mutex_lock(&((os_event_t *)event)->mutex);
    pthread_cond_signal(&((os_event_t *)event)->cond);
    pthread_mutex_unlock(&((os_event_t *)event)->mutex);

    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t posix_vdev_os_event_wait(
        _IN_ vdev_os_event_t event)
{
    pthread_mutex_lock(&((os_event_t *)event)->mutex);
    pthread_cond_wait(&((os_event_t *)event)->cond, &((os_event_t *)event)->mutex);
    pthread_mutex_unlock(&((os_event_t *)event)->mutex);

    return VDEV_STATUS_SUCCESS;
}

static uint32_t posix_vdev_get_task_id(void)
{
    return (uint32_t)pthread_self();
}

static void posix_sleep(uint32_t sec)
{
    sleep(sec);
}

static void posix_msleep(uint32_t ms)
{
    usleep(ms * 1000);
}

static uint32_t posix_get_time(void)
{
    struct timeval tc;

    gettimeofday(&tc, NULL);

    return ((tc.tv_sec - pOsInfo->ts.tv_sec) * 1000 +
            (tc.tv_usec - pOsInfo->ts.tv_usec) / 1000);
}

void vdev_os_api_install(vdev_os_api_t *api)
{
    api->init          = posix_vdev_os_init;
    api->destroy       = posix_vdev_os_destroy;
    api->task_create   = posix_vdev_os_task_create;
    api->task_delete   = posix_vdev_os_task_delete;
    api->mutex_create  = posix_vdev_os_mutex_create;
    api->mutex_delete  = posix_vdev_os_mutex_delete;
    api->mutex_lock    = posix_vdev_os_mutex_lock;
    api->mutex_unlock  = posix_vdev_os_mutex_unlock;
    api->event_create = posix_vdev_os_event_create;
    api->event_delete = posix_vdev_os_event_delete;
    api->event_set    = posix_vdev_os_event_set;
    api->event_wait   = posix_vdev_os_event_wait;
    api->get_task_id   = posix_vdev_get_task_id;
    api->sleep         = posix_sleep;
    api->msleep        = posix_msleep;
    api->get_time      = posix_get_time;

    vdev_os_queue_api_install(api);
}

#endif

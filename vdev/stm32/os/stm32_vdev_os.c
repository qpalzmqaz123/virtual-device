#include "vdev.h"
#include <stdlib.h>
#include <string.h>
#include "stm32_vdev_os.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"


static vdev_status_t stm32_vdev_os_init(void)
{
    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t stm32_vdev_os_destroy(void)
{
    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t stm32_vdev_os_task_create(
        _OUT_ vdev_os_task_t *task,
        _IN_ void (*fn)(void *arg),
        _IN_ void *arg,
        _IN_ const char *name)
{
    xTaskCreate(fn, name, 1024, NULL, tskIDLE_PRIORITY + 3, NULL);
    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t stm32_vdev_os_task_delete(
        _IN_ vdev_os_task_t task)
{
    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t stm32_vdev_os_task_start(void)
{
    vTaskStartScheduler();
    return VDEV_STATUS_SUCCESS;
}


static vdev_status_t stm32_vdev_os_mutex_create(
        _OUT_ vdev_os_mutex_t *mutex)
{
    *mutex = xSemaphoreCreateMutex();
    VDEV_RETURN_IF_NULL(*mutex, VDEV_STATUS_OUT_OF_MEMORY, "Can't create mutex");

    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t stm32_vdev_os_mutex_delete(
        _IN_ vdev_os_mutex_t mutex)
{
    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t stm32_vdev_os_mutex_lock(
        _IN_ vdev_os_mutex_t mutex)
{
    xSemaphoreTake(mutex, portMAX_DELAY);
    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t stm32_vdev_os_mutex_unlock(
        _IN_ vdev_os_mutex_t mutex)
{
    xSemaphoreGive(mutex);
    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t stm32_vdev_os_event_create(
        _OUT_ vdev_os_event_t *event)
{
    vSemaphoreCreateBinary(*event);
    VDEV_RETURN_IF_NULL(*event, VDEV_STATUS_OUT_OF_MEMORY, "Can't create event");
    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t stm32_vdev_os_event_delete(
        _IN_ vdev_os_event_t event)
{
    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t stm32_vdev_os_event_set(
        _IN_ vdev_os_event_t event)
{
    xSemaphoreGive(event);
    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t stm32_vdev_os_event_wait(
        _IN_ vdev_os_event_t event)
{
    while (xSemaphoreTake(event, portMAX_DELAY) != pdTRUE);
    return VDEV_STATUS_SUCCESS;
}

static uint32_t stm32_vdev_get_task_id(void)
{
    return ((uint32_t) xTaskGetCurrentTaskHandle());
    return 0;
}

static void stm32_sleep(
        _IN_ uint32_t sec)
{
    vTaskDelay(sec * 1000);
}

static void stm32_msleep(
        _IN_ uint32_t ms)
{
    vTaskDelay(ms);
}

static uint32_t stm32_get_time(void)
{
    return ((uint32_t) xTaskGetTickCount());
}

void
vdev_os_api_install(vdev_os_api_t *api)
{
    api->init          = stm32_vdev_os_init;
    api->destroy       = stm32_vdev_os_destroy;
    api->task_create   = stm32_vdev_os_task_create;
    api->task_delete   = stm32_vdev_os_task_delete;
    api->task_start    = stm32_vdev_os_task_start;
    api->mutex_create  = stm32_vdev_os_mutex_create;
    api->mutex_delete  = stm32_vdev_os_mutex_delete;
    api->mutex_lock    = stm32_vdev_os_mutex_lock;
    api->mutex_unlock  = stm32_vdev_os_mutex_unlock;
    api->event_create  = stm32_vdev_os_event_create;
    api->event_delete  = stm32_vdev_os_event_delete;
    api->event_set     = stm32_vdev_os_event_set;
    api->event_wait    = stm32_vdev_os_event_wait;
    api->get_task_id   = stm32_vdev_get_task_id;
    api->sleep         = stm32_sleep;
    api->msleep        = stm32_msleep;
    api->get_time      = stm32_get_time;
}


#include <pthread.h>
#include "vdev.h"
#include "vdev_model.h"
#include "posix_manager.h"
#include "uthash/uthash.h"

typedef enum _button_cmd_t {
    BUTTON_CMD_INIT       = 0,
    BUTTON_CMD_IS_PRESSED = 1,
    BUTTON_CMD_PRESS      = 2,
    BUTTON_CMD_RELEASE    = 3,

    BUTTON_CMD_MAX
} button_cmd_t;

typedef struct _button_t {
    int                 id;
    posix_manager_key_t key;
    UT_hash_handle      hh;

    bool                is_pressed;
    vdev_async_callback press_cb;
    void *              press_args;
    vdev_async_callback release_cb;
    void *              release_args;
    pthread_t           thread;
} button_t;


static button_t *pHead = NULL;

static void *
posix_vdev_button_receive_thread(void *args)
{
    uint8_t res;
    button_t *button = (button_t *)args;
    vdev_async_callback cb;
    void *cb_args = NULL;

    while (1) {
        cb = 0;

        posix_manager_recv(&button->key, &res, sizeof(uint8_t));

        switch (res) {
            case BUTTON_CMD_PRESS:
                button->is_pressed = true;
                cb = button->press_cb;
                cb_args = button->press_args;
                break;
            case BUTTON_CMD_RELEASE:
                button->is_pressed = false;
                cb = button->release_cb;
                cb_args = button->release_args;
                break;
            default:
                abort(); /* unsupported command */
                break;
        }

        if (cb) {
            cb(VDEV_STATUS_SUCCESS, cb_args);
        }
    }

    return NULL;
}

static vdev_status_t
posix_vdev_button_init(
        _IN_ uint32_t id)
{
    button_t *button = NULL;
    uint8_t cmd = BUTTON_CMD_INIT;
    uint8_t res;

    VDEV_RETURN_IF_NULL(button = (button_t *)malloc(sizeof(button_t)), VDEV_STATUS_OUT_OF_MEMORY, "");
    memset(button, 0, sizeof(button_t));

    button->id = id;
    button->key.id = id;
    button->key.model = VDEV_MODEL_BUTTON;
    HASH_ADD_INT(pHead, id, button);

    posix_manager_register(&button->key);

    posix_manager_send(&button->key, &cmd, sizeof(uint8_t));
    posix_manager_recv(&button->key, &res, sizeof(uint8_t));

    /* create receive thread */
    pthread_create(&button->thread, NULL, posix_vdev_button_receive_thread, button);

    return res;
}

static vdev_status_t
posix_vdev_button_is_pressed(
        _IN_ uint32_t id,
        _OUT_ bool *is_pressed)
{
    button_t *button = NULL;
    HASH_FIND_INT(pHead, &id, button);

    *is_pressed = button->is_pressed;

    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t
posix_vdev_button_on_press(
        _IN_ uint32_t id,
        _IN_ vdev_async_callback cb,
        _IN_ void *args)
{
    button_t *button = NULL;
    HASH_FIND_INT(pHead, &id, button);

    button->press_cb = cb;
    button->press_args = args;

    return VDEV_STATUS_SUCCESS;
}

static vdev_status_t
posix_vdev_button_on_release(
        _IN_ uint32_t id,
        _IN_ vdev_async_callback cb,
        _IN_ void *args)
{
    button_t *button = NULL;
    HASH_FIND_INT(pHead, &id, button);

    button->release_cb = cb;
    button->release_args = args;

    return VDEV_STATUS_SUCCESS;
}

void
vdev_button_api_install(vdev_button_api_t *api)
{
    api->init = posix_vdev_button_init;
    api->is_pressed = posix_vdev_button_is_pressed;
    api->on_press = posix_vdev_button_on_press;
    api->on_release = posix_vdev_button_on_release;
}


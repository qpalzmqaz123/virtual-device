#include "vdev.h"
#include "vdev_model.h"
#include "posix_manager.h"
#include "uthash/include/uthash.h"


#define SDCARD_FILE_NAMESIZE 32

#define GEN_SDCARD_FILENAME(buf, id) do { \
    sprintf(buf, "sdcard_%d.bin", (id)); \
} while(0)


typedef struct _sdcard_t {
    int                id;
    FILE              *fp;
    char               name[SDCARD_FILE_NAMESIZE];
    vdev_sdcard_info_t info;
    UT_hash_handle     hh;
} sdcard_t;


static sdcard_t *pHead = NULL;


vdev_status_t posix_vdev_sdcard_init(
        _IN_ uint32_t id)
{
    sdcard_t *p_sd;

    VDEV_RETURN_IF_NULL(p_sd = (sdcard_t *)malloc(sizeof(sdcard_t)), VDEV_STATUS_OUT_OF_MEMORY, "");
    memset(p_sd, 0, sizeof(sdcard_t));

    p_sd->id = id;
    GEN_SDCARD_FILENAME(p_sd->name, id);
    p_sd->info.capacity = VDEV_SIMULATION_SDCARD_CAPACITY;
    p_sd->info.block_size = VDEV_SIMULATION_SDCARD_BLOCKSIZE;
    p_sd->info.exist = true;
    p_sd->info.protected = false;

    HASH_ADD_INT(pHead, id, p_sd);

    p_sd->fp = fopen(p_sd->name, "a+");

    return VDEV_STATUS_SUCCESS;
}

vdev_status_t posix_vdev_sdcard_read(
        _IN_ uint32_t id,
        _OUT_ void *buffer,
        _IN_ uint32_t selector,
        _IN_ uint32_t count)
{
    size_t size;
    uint64_t offset;
    sdcard_t *p_sd;

    HASH_FIND_INT(pHead, &id, p_sd);

    offset = p_sd->info.block_size * selector;
    fseek(p_sd->fp, offset, SEEK_SET);
    size = fread(buffer, 1, p_sd->info.block_size * count, p_sd->fp);

    return size == p_sd->info.block_size * count ? VDEV_STATUS_SUCCESS : VDEV_STATUS_FAILURE;
}

vdev_status_t posix_vdev_sdcard_write(
        _IN_ uint32_t id,
        _IN_ const void *buffer,
        _IN_ uint32_t selector,
        _IN_ uint32_t count)
{
    size_t size;
    uint64_t offset;
    sdcard_t *p_sd;

    HASH_FIND_INT(pHead, &id, p_sd);

    offset = p_sd->info.block_size * selector;
    fseek(p_sd->fp, offset, SEEK_SET);
    size = fwrite(buffer, 1, p_sd->info.block_size * count, p_sd->fp);
    fflush(p_sd->fp);

    return size == p_sd->info.block_size * count ? VDEV_STATUS_SUCCESS : VDEV_STATUS_FAILURE;
}

vdev_sdcard_info_t *posix_vdev_sdcard_get_info(
        _IN_ uint32_t id)
{
    sdcard_t *p_sd;

    HASH_FIND_INT(pHead, &id, p_sd);

    return &p_sd->info;
}

void
vdev_sdcard_api_install(vdev_sdcard_api_t *p_api)
{
    p_api->init     = posix_vdev_sdcard_init;
    p_api->read     = posix_vdev_sdcard_read;
    p_api->write    = posix_vdev_sdcard_write;
    p_api->get_info = posix_vdev_sdcard_get_info;
}


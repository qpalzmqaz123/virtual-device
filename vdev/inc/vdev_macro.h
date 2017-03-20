#ifndef __VDEV_MACRO_H__
#define __VDEV_MACRO_H__

#define VDEV_LOG_TYPE_TO_STRING(type) ( \
        type == VDEV_LOG_CRITICAL ? "CRITICAL: " : \
        type == VDEV_LOG_ERROR    ? "ERROR: "    : \
        type == VDEV_LOG_WARING   ? "WARING: "   : \
        type == VDEV_LOG_INFO     ? "INFO: "     : \
        type == VDEV_LOG_DEBUG    ? "DEBUG: "    : \
        "UNKNOW TYPE: ")

#if VDEV_SUPPORT_LOG
#define VDEV_LOG(type, fmt, ...) do { \
    if (type >= vdev_get_api()->log) { \
        VDEV_LOG_PRESENT(VDEV_LOG_TYPE_TO_STRING(type)); \
        VDEV_LOG_PRESENT(fmt"\n", ##__VA_ARGS__); \
    } \
} while (0)
#else
#define VDEV_LOG(...)
#endif

#define VDEV_ASSERT_NOT_NULL(ptr) do { \
    if (NULL == (ptr)) { \
        VDEV_LOG(VDEV_LOG_CRITICAL, "ASSERT_NOT_NULL failed at %s:%d", __FILE__, __LINE__); \
        VDEV_ABORT(); \
    } \
} while (0)

#define VDEV_ASSERT_SUCCESS(ret) do { \
    if (ret) { \
        VDEV_LOG(VDEV_LOG_CRITICAL, "ASSERT_SUCCESS failed at %s:%d", __FILE__, __LINE__); \
        VDEV_ABORT(); \
    } \
} while (0)

#define VDEV_RETURN_IF_ERROR(res, error, fmt, ...) do { \
    if (res) { \
        VDEV_LOG(VDEV_LOG_ERROR, fmt, ##__VA_ARGS__); \
        return (error); \
    } \
} while (0) \

#define VDEV_RETURN_IF_NULL(res, error, fmt, ...) do { \
    if (res) { \
        VDEV_LOG(VDEV_LOG_ERROR, fmt, ##__VA_ARGS__); \
        return (error); \
    } \
} while (0) \

#endif

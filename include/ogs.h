#ifndef OGS_H
#define OGS_H

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_VERSION_MAJOR 0
#define OGS_VERSION_MINOR 1
#define OGS_VERSION_PATCH 0

#define OGS_MAKE_VERSION(major, minor, patch)                                  \
    (((uint32_t)(major) << 16) | ((uint32_t)(minor) << 8) | (uint32_t)(patch))

#define OGS_VERSION                                                            \
    OGS_MAKE_VERSION(OGS_VERSION_MAJOR, OGS_VERSION_MINOR, OGS_VERSION_PATCH)

typedef enum OgsResult {
    OGS_SUCCESS = 0,
    OGS_ERROR_INVALID_ARGUMENT = -1,
    OGS_ERROR_OUT_OF_MEMORY = -2,
    OGS_ERROR_VULKAN_INIT_FAILED = -3,
    OGS_ERROR_NO_SUITABLE_DEVICE = -4,
} OgsResult;

// The core context. All OGS operations require a valid OgsContext*.
typedef struct OgsContext OgsContext;
OgsContext *ogsInit(void);
void ogsShutdown(OgsContext *ctx);

// Opaque handle for GPU buffers. Details are hidden from the public API.
typedef struct OgsBuffer OgsBuffer;
OgsBuffer *ogsCreateBuffer(OgsContext *ctx, size_t size, OgsResult *out_result);
void ogsDestroyBuffer(OgsContext *ctx, OgsBuffer *buffer);

const char *ogsGetErrorString(OgsResult result);
void ogsSetLogSink(FILE *sink);

#ifdef __cplusplus
}
#endif

#endif /* OGS_H */

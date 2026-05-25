#ifndef OGS_INTERNAL_H
#define OGS_INTERNAL_H

/*
 * ogs_internal.h - Private implementation types.
 *
 * This header MUST NOT be included from any public-facing header.
 * It is the only header where Vulkan types appear directly.
 */

#include "ogs.h"
#include <stdio.h>
#include <vulkan/vulkan.h>

extern FILE *_OGS_LOG_SINK;
#define OGS_LOG(fmt, ...)                                                      \
    do {                                                                       \
        if (_OGS_LOG_SINK != NULL) {                                           \
            fprintf(_OGS_LOG_SINK, "[OGS] " fmt "\n", __VA_ARGS__);            \
        }                                                                      \
    } while (0)
#define OGS_LOG_STR(fmt) OGS_LOG("%s", fmt)

void _ogsInitLog(void);

struct OgsBuffer {
    VkDevice device;
    VkBuffer buffer;
    VkDeviceMemory memory;
    size_t size;
};

struct OgsContext {
    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkQueue computeQueue;
    uint32_t computeQueueFamilyIndex;

#ifdef OGS_DEBUG
    VkDebugUtilsMessengerEXT debugMessenger;
#endif
};

#endif /* OGS_INTERNAL_H */

#ifndef OGS_INTERNAL_H
#define OGS_INTERNAL_H

/*
 * ogs_internal.h - Private implementation types.
 *
 * This header MUST NOT be included from any public-facing header.
 * It is the only place where Vulkan types appear directly.
 */

#include "ogs.h"
#include <vulkan/vulkan.h>

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

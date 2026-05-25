#include "ogs_internal.h"

#include <stdlib.h>
#include <string.h>

static uint32_t _ogsFindMemoryType(OgsContext *ctx, uint32_t type_filter,
                                   VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memory_properties;
    vkGetPhysicalDeviceMemoryProperties(ctx->physicalDevice,
                                        &memory_properties);

    for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++) {
        if ((type_filter & (1u << i)) &&
            (memory_properties.memoryTypes[i].propertyFlags & properties) ==
                properties) {
            return i;
        }
    }

    return UINT32_MAX;
}

OgsBuffer *ogsCreateBuffer(OgsContext *ctx, size_t size,
                           OgsResult *out_result) {
    if (out_result) {
        *out_result = OGS_ERROR_INVALID_ARGUMENT;
    }

    if (ctx == NULL || size == 0) {
        return NULL;
    }

    OgsBuffer *buffer = calloc(1, sizeof(OgsBuffer));
    if (buffer == NULL) {
        if (out_result) {
            *out_result = OGS_ERROR_OUT_OF_MEMORY;
        }
        return NULL;
    }

    buffer->device = ctx->device;
    buffer->size = size;

    VkBufferCreateInfo buffer_ci = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = size,
        .usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
                 VK_BUFFER_USAGE_TRANSFER_SRC_BIT |
                 VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };

    VkResult vk_result =
        vkCreateBuffer(ctx->device, &buffer_ci, NULL, &buffer->buffer);
    if (vk_result != VK_SUCCESS) {
        OGS_LOG("vkCreateBuffer failed: %d", vk_result);
        free(buffer);
        if (out_result) {
            *out_result = OGS_ERROR_VULKAN_INIT_FAILED;
        }
        return NULL;
    }

    VkMemoryRequirements memory_requirements;
    vkGetBufferMemoryRequirements(ctx->device, buffer->buffer,
                                  &memory_requirements);

    uint32_t memory_type_index =
        _ogsFindMemoryType(ctx, memory_requirements.memoryTypeBits,
                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                               VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    if (memory_type_index == UINT32_MAX) {
        OGS_LOG_STR(
            "No suitable host-visible memory type for buffer allocation.");
        vkDestroyBuffer(ctx->device, buffer->buffer, NULL);
        free(buffer);
        if (out_result) {
            *out_result = OGS_ERROR_VULKAN_INIT_FAILED;
        }
        return NULL;
    }

    VkMemoryAllocateInfo alloc_info = {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = memory_requirements.size,
        .memoryTypeIndex = memory_type_index,
    };

    vk_result =
        vkAllocateMemory(ctx->device, &alloc_info, NULL, &buffer->memory);
    if (vk_result != VK_SUCCESS) {
        OGS_LOG("vkAllocateMemory failed: %d", vk_result);
        vkDestroyBuffer(ctx->device, buffer->buffer, NULL);
        free(buffer);
        if (out_result) {
            *out_result = OGS_ERROR_OUT_OF_MEMORY;
        }
        return NULL;
    }

    vk_result =
        vkBindBufferMemory(ctx->device, buffer->buffer, buffer->memory, 0);
    if (vk_result != VK_SUCCESS) {
        OGS_LOG("vkBindBufferMemory failed: %d", vk_result);
        vkFreeMemory(ctx->device, buffer->memory, NULL);
        vkDestroyBuffer(ctx->device, buffer->buffer, NULL);
        free(buffer);
        if (out_result) {
            *out_result = OGS_ERROR_VULKAN_INIT_FAILED;
        }
        return NULL;
    }

    void *mapped_memory = NULL;
    vk_result =
        vkMapMemory(ctx->device, buffer->memory, 0, size, 0, &mapped_memory);
    if (vk_result == VK_SUCCESS && mapped_memory != NULL) {
        memset(mapped_memory, 0, size);
        vkUnmapMemory(ctx->device, buffer->memory);
    }

    if (out_result) {
        *out_result = OGS_SUCCESS;
    }

    return buffer;
}

void ogsDestroyBuffer(OgsContext *ctx, OgsBuffer *buffer) {
    (void)ctx;

    if (buffer == NULL) {
        return;
    }

    if (buffer->device != VK_NULL_HANDLE) {
        if (buffer->memory != VK_NULL_HANDLE) {
            vkFreeMemory(buffer->device, buffer->memory, NULL);
        }
        if (buffer->buffer != VK_NULL_HANDLE) {
            vkDestroyBuffer(buffer->device, buffer->buffer, NULL);
        }
    }

    free(buffer);
}

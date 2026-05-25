#include "ogs_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* -------------------------------------------------------------------------
 * Debug utilities (compiled out in Release builds)
 * ---------------------------------------------------------------------- */

#ifdef OGS_DEBUG

static const char *const VALIDATION_LAYERS[] = {"VK_LAYER_KHRONOS_validation"};
static const uint32_t VALIDATION_LAYER_COUNT = 1;

static VKAPI_ATTR VkBool32 VKAPI_CALL _ogsDebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT *data, void *user_data) {
    (void)type;
    (void)user_data;
    if (severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        OGS_LOG("[Vulkan] %s", data->pMessage);
    }
    return VK_FALSE;
}

static int _ogsCheckValidationLayerSupport(void) {
    uint32_t layer_count = 0;
    vkEnumerateInstanceLayerProperties(&layer_count, NULL);

    VkLayerProperties *layers = malloc(sizeof(VkLayerProperties) * layer_count);
    if (!layers) return 0;
    vkEnumerateInstanceLayerProperties(&layer_count, layers);

    for (uint32_t i = 0; i < VALIDATION_LAYER_COUNT; i++) {
        int found = 0;
        for (uint32_t j = 0; j < layer_count; j++) {
            if (strcmp(VALIDATION_LAYERS[i], layers[j].layerName) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            free(layers);
            return 0;
        }
    }

    free(layers);
    return 1;
}

#endif /* OGS_DEBUG */

/* -------------------------------------------------------------------------
 * Physical device selection helpers
 * ---------------------------------------------------------------------- */

/*
 * _ogsFindComputeQueueFamily - Finds the first queue family with compute
 * support. Returns 1 and writes the family index to *out_index on success, 0 on
 * failure.
 */
static int _ogsFindComputeQueueFamily(VkPhysicalDevice physical_device,
                                      uint32_t *out_index) {
    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, NULL);

    VkQueueFamilyProperties *families =
        malloc(sizeof(VkQueueFamilyProperties) * count);
    if (!families) return 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, families);

    int found = 0;
    for (uint32_t i = 0; i < count; i++) {
        if (families[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
            *out_index = i;
            found = 1;
            break;
        }
    }

    free(families);
    return found;
}

/*
 * _ogsSelectPhysicalDevice - Enumerates devices and picks the best one.
 * Prefers a discrete GPU; falls back to any device with compute support.
 * Writes the compute queue family index to *out_compute_family on success.
 */
static VkPhysicalDevice _ogsSelectPhysicalDevice(VkInstance instance,
                                                 uint32_t *out_compute_family) {
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance, &device_count, NULL);
    if (device_count == 0) return VK_NULL_HANDLE;

    VkPhysicalDevice *devices = malloc(sizeof(VkPhysicalDevice) * device_count);
    if (!devices) return VK_NULL_HANDLE;
    vkEnumeratePhysicalDevices(instance, &device_count, devices);

    VkPhysicalDevice selected = VK_NULL_HANDLE;
    uint32_t family = 0;

    /* First pass: prefer discrete GPU */
    for (uint32_t i = 0; i < device_count; i++) {
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(devices[i], &props);
        if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            if (_ogsFindComputeQueueFamily(devices[i], &family)) {
                selected = devices[i];
                break;
            }
        }
    }

    /* Second pass: accept any device with compute support */
    if (selected == VK_NULL_HANDLE) {
        for (uint32_t i = 0; i < device_count; i++) {
            if (_ogsFindComputeQueueFamily(devices[i], &family)) {
                selected = devices[i];
                break;
            }
        }
    }

    free(devices);

    if (selected != VK_NULL_HANDLE) {
        *out_compute_family = family;

#ifdef OGS_DEBUG
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(selected, &props);
        OGS_LOG("Selected device: %s", props.deviceName);
#endif
    }

    return selected;
}

/* -------------------------------------------------------------------------
 * Public API
 * ---------------------------------------------------------------------- */

OgsContext *ogsInit(void) {
    _ogsInitLog();
    OgsContext *ctx = calloc(1, sizeof(OgsContext));
    if (!ctx) return NULL;

    /* --- Vulkan Instance --- */
    VkApplicationInfo app_info = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "OpenGaussSplat",
        .applicationVersion = VK_MAKE_VERSION(
            OGS_VERSION_MAJOR, OGS_VERSION_MINOR, OGS_VERSION_PATCH),
        .pEngineName = "OGS",
        .engineVersion = VK_MAKE_VERSION(OGS_VERSION_MAJOR, OGS_VERSION_MINOR,
                                         OGS_VERSION_PATCH),
        .apiVersion = VK_API_VERSION_1_3,
    };

    VkInstanceCreateInfo instance_ci = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &app_info,
    };

#ifdef OGS_DEBUG
    const char *debug_extensions[] = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME};
    if (_ogsCheckValidationLayerSupport()) {
        instance_ci.enabledLayerCount = VALIDATION_LAYER_COUNT;
        instance_ci.ppEnabledLayerNames = VALIDATION_LAYERS;
        instance_ci.enabledExtensionCount = 1;
        instance_ci.ppEnabledExtensionNames = debug_extensions;
    } else {
        OGS_LOG("Validation layers requested but not available.");
    }
#endif

    if (vkCreateInstance(&instance_ci, NULL, &ctx->instance) != VK_SUCCESS) {
        free(ctx);
        return NULL;
    }

#ifdef OGS_DEBUG
    /* Attach debug messenger */
    PFN_vkCreateDebugUtilsMessengerEXT fn_create =
        (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            ctx->instance, "vkCreateDebugUtilsMessengerEXT");
    if (fn_create) {
        VkDebugUtilsMessengerCreateInfoEXT debug_ci = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback = _ogsDebugCallback,
        };
        fn_create(ctx->instance, &debug_ci, NULL, &ctx->debugMessenger);
    }
#endif

    /* --- Physical Device --- */
    ctx->physicalDevice =
        _ogsSelectPhysicalDevice(ctx->instance, &ctx->computeQueueFamilyIndex);
    if (ctx->physicalDevice == VK_NULL_HANDLE) {
        vkDestroyInstance(ctx->instance, NULL);
        free(ctx);
        return NULL;
    }

    /* --- Logical Device --- */
    float queue_priority = 1.0f;
    VkDeviceQueueCreateInfo queue_ci = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = ctx->computeQueueFamilyIndex,
        .queueCount = 1,
        .pQueuePriorities = &queue_priority,
    };

    VkDeviceCreateInfo device_ci = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos = &queue_ci,
    };

    if (vkCreateDevice(ctx->physicalDevice, &device_ci, NULL, &ctx->device) !=
        VK_SUCCESS) {
        vkDestroyInstance(ctx->instance, NULL);
        free(ctx);
        return NULL;
    }

    vkGetDeviceQueue(ctx->device, ctx->computeQueueFamilyIndex, 0,
                     &ctx->computeQueue);

    return ctx;
}

void ogsShutdown(OgsContext *ctx) {
    if (!ctx) return;

    if (ctx->device != VK_NULL_HANDLE) {
        vkDeviceWaitIdle(ctx->device);
        vkDestroyDevice(ctx->device, NULL);
    }

#ifdef OGS_DEBUG
    if (ctx->debugMessenger != VK_NULL_HANDLE) {
        PFN_vkDestroyDebugUtilsMessengerEXT fn_destroy =
            (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
                ctx->instance, "vkDestroyDebugUtilsMessengerEXT");
        if (fn_destroy) {
            fn_destroy(ctx->instance, ctx->debugMessenger, NULL);
        }
    }
#endif

    if (ctx->instance != VK_NULL_HANDLE) {
        vkDestroyInstance(ctx->instance, NULL);
    }

    free(ctx);
}

const char *ogsGetErrorString(OgsResult result) {
    switch (result) {
    case OGS_SUCCESS: return "OGS_SUCCESS";
    case OGS_ERROR_INVALID_ARGUMENT: return "OGS_ERROR_INVALID_ARGUMENT";
    case OGS_ERROR_OUT_OF_MEMORY: return "OGS_ERROR_OUT_OF_MEMORY";
    case OGS_ERROR_VULKAN_INIT_FAILED: return "OGS_ERROR_VULKAN_INIT_FAILED";
    case OGS_ERROR_NO_SUITABLE_DEVICE: return "OGS_ERROR_NO_SUITABLE_DEVICE";
    default: return "OGS_ERROR_UNKNOWN";
    }
}

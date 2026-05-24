#ifndef OGS_H
#define OGS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * OpenGaussSplat (OGS) - Public API
 *
 * All public symbols are prefixed with `ogs`. Vulkan types are intentionally
 * kept out of this header; see src/ogs_internal.h for the concrete definitions.
 */

/* -------------------------------------------------------------------------
 * Version
 * ---------------------------------------------------------------------- */
#define OGS_VERSION_MAJOR 0
#define OGS_VERSION_MINOR 1
#define OGS_VERSION_PATCH 0

#define OGS_MAKE_VERSION(major, minor, patch) \
    (((uint32_t)(major) << 16) | ((uint32_t)(minor) << 8) | (uint32_t)(patch))

#define OGS_VERSION \
    OGS_MAKE_VERSION(OGS_VERSION_MAJOR, OGS_VERSION_MINOR, OGS_VERSION_PATCH)

/* -------------------------------------------------------------------------
 * Result codes
 * ---------------------------------------------------------------------- */
typedef enum OgsResult {
    OGS_SUCCESS                    = 0,
    OGS_ERROR_OUT_OF_MEMORY        = -1,
    OGS_ERROR_VULKAN_INIT_FAILED   = -2,
    OGS_ERROR_NO_SUITABLE_DEVICE   = -3,
} OgsResult;

/* -------------------------------------------------------------------------
 * Opaque handles
 * ---------------------------------------------------------------------- */

/* The core context. All OGS operations require a valid OgsContext*. */
typedef struct OgsContext OgsContext;

/* -------------------------------------------------------------------------
 * Core lifecycle
 * ---------------------------------------------------------------------- */

/*
 * ogsInit - Initialize the OGS context and underlying Vulkan resources.
 *
 * Selects the best available compute-capable GPU (prefers discrete).
 * In Debug builds (OGS_DEBUG defined), Vulkan validation layers and the
 * debug messenger are enabled automatically.
 *
 * Returns a heap-allocated OgsContext* on success, or NULL on failure.
 */
OgsContext* ogsInit(void);

/*
 * ogsShutdown - Destroy all Vulkan resources and free the context.
 *
 * Safe to call with NULL.
 */
void ogsShutdown(OgsContext* ctx);

/* -------------------------------------------------------------------------
 * Utilities
 * ---------------------------------------------------------------------- */

/* Returns a human-readable string for the given OgsResult. */
const char* ogsGetErrorString(OgsResult result);

#ifdef __cplusplus
}
#endif

#endif /* OGS_H */

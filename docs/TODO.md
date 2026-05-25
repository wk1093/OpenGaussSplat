# OpenGaussSplat TODO Roadmap

## Features

- [ ] Add `ogsCreateBuffer`/`ogsDestroyBuffer` API skeleton (2-4h)
  - [x] Introduce opaque `OgsBuffer` handle in public API.
  - [ ] Add create/destroy declarations in `ogs.h`.
  - [ ] Wire minimal internal allocation/free path with clear error returns.

- [ ] Add first `ogsRender` public entrypoint (2-4h)
  - [ ] Add minimal render params struct (viewport width/height + camera placeholder).
  - [ ] Validate context and buffer handles.
  - [ ] Return deterministic "not implemented yet" or success code path.

- [ ] Implement command pool + primary command buffer creation (2-4h)
  - [ ] Allocate per-context command pool on init.
  - [ ] Allocate one reusable primary command buffer.
  - [ ] Destroy resources in shutdown.

- [ ] Add GPU buffer upload path for raw splat data (2-4h)
  - [ ] Add staging buffer helper.
  - [ ] Copy host data to device-local buffer.
  - [ ] Keep API simple: one call for create+upload.

- [ ] Add first compute pipeline bootstrap function (2-4h)
  - [ ] Create descriptor set layout for input/output buffers.
  - [ ] Create pipeline layout.
  - [ ] Stub compute pipeline creation with TODO shader path.

- [ ] Add descriptor pool + descriptor set allocation helper (2-3h)
  - [ ] Create one pool sized for MVP rendering path.
  - [ ] Allocate one descriptor set per context.
  - [ ] Add destroy path in shutdown.

- [ ] Record and submit first no-op compute dispatch (2-3h)
  - [ ] Begin/end command buffer safely.
  - [ ] Bind pipeline + descriptor set if available.
  - [ ] Submit to compute queue and wait idle.

- [ ] Add minimal camera struct to public API (1-2h)
  - [ ] Define position + forward/up vectors.
  - [ ] Keep it POD and C ABI-safe.
  - [ ] Thread through `ogsRender` signature.

- [ ] Add simple render target abstraction (`OgsImage`) (2-4h)
  - [ ] Create/destroy image + image view helpers.
  - [ ] Keep format fixed for first version.
  - [ ] Store in context for now (single target path).

- [ ] Add first end-to-end sample in `examples/` (2-3h)
  - [ ] Init context.
  - [ ] Create test buffer with fake splat data.
  - [ ] Call render once and print status.

## Feature-Adjacent UX and API Improvements

- [ ] Add `ogsGetVersion` and `ogsGetVersionString` (1-2h)
  - [ ] Return runtime major/minor/patch.
  - [ ] Return stable semantic version string.
  - [ ] Add basic test assertions.

- [ ] Add structured render status codes (2-3h)
  - [ ] Add `OGS_ERROR_NOT_READY` and `OGS_ERROR_INVALID_ARGUMENT`.
  - [ ] Use these in buffer and render APIs.
  - [ ] Map to `ogsGetErrorString`.

- [ ] Add `ogsDumpDeviceInfo` utility for users (2-3h)
  - [ ] Expose API to print selected GPU properties.
  - [ ] Include queue family index and API version.
  - [ ] Add docs snippet in README.

- [ ] Add `ogsSetLogSink` usage example to README (1h)
  - [ ] Show redirect to `stderr`.
  - [ ] Show disabling logs with `NULL`.
  - [ ] Clarify default behavior.

- [ ] Add explicit "hello render" quickstart section (2h)
  - [ ] Show full minimal C example using upcoming API.
  - [ ] Include build + run commands.
  - [ ] Include expected output.

## Functional Test Tasks

- [ ] Add smoke test for buffer create/destroy lifecycle (2-3h)
  - [ ] Create buffer with tiny synthetic data.
  - [ ] Destroy and verify no crash.
  - [ ] Ensure repeated create/destroy works.

- [ ] Add smoke test for one compute dispatch submit (2-3h)
  - [ ] Init context.
  - [ ] Record/submit command buffer.
  - [ ] Assert return code success.

- [ ] Add API argument validation tests for feature calls (2-3h)
  - [ ] Null context checks.
  - [ ] Null buffer/data checks.
  - [ ] Invalid size checks.

- [ ] Split `ogs_test` into focused test executables (2-4h)
  - [ ] Keep init test standalone.
  - [ ] Add buffer test executable.
  - [ ] Add render/dispatch test executable.

## Supporting Maintenance

- [ ] Centralize partial-init cleanup helper (1-2h)
  - [ ] Create one internal cleanup function.
  - [ ] Use it from feature init failure paths.
  - [ ] Confirm no leaks in buffer/pipeline setup paths.

- [ ] Add Vulkan result logging helper for faster debugging (1-2h)
  - [ ] Add `_ogsVkResultToString`.
  - [ ] Include function context in logs.
  - [ ] Use only where feature work touches Vulkan calls.

- [ ] Add CMake option for sanitizers in Debug (2-3h)
  - [ ] Add `OGS_ENABLE_SANITIZERS` option.
  - [ ] Apply compile + link flags.
  - [ ] Document usage for feature debugging.

- [ ] Add warnings baseline for changed feature files (1-2h)
  - [ ] Set warning flags for GCC/Clang.
  - [ ] Keep non-fatal initially.
  - [ ] Ensure new feature files build warning-free.
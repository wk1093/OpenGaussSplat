# Test Procedure

This directory uses one small executable per smoke test.

## File Format

- Name new test files `ogs_test_*.c`.
- Include `ogs_test_support.h` for the common start/fail/finish helpers.
- Keep each file focused on one behavior area.
- Return `0` on success and `1` on the first failed check.

## How Discovery Works

- `tests/CMakeLists.txt` uses `file(GLOB CONFIGURE_DEPENDS ...)` to find every `ogs_test*.c` file.
- Each discovered file becomes its own executable target.
- Each executable is registered with CTest automatically.

## How To Run

- Build everything: `cmake --build build`
- Run all tests through CTest: `ctest --test-dir build --output-on-failure`
- Build and run the generic test target: `cmake --build build --target check`

## How To Add A New Smoke Test

1. Create a new file such as `ogs_test_pipeline.c`.
2. Include `ogs.h` and `ogs_test_support.h`.
3. Write one `main(void)` that exercises a single behavior.
4. Use `OGS_TEST_ASSERT` for checks.
5. Rebuild. CMake will discover the new file automatically.
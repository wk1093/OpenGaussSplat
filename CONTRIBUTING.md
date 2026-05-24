# Contributing to OpenGaussSplat

We welcome contributions to OpenGaussSplat! To maintain the project's high performance and readability, please adhere to the following coding standards and project structure.

## Code Style Guidelines

TODO: Eventually we will setup an auto-formatter (e.g., clang-format) to enforce these rules, but for now please follow them (or I will clean them up, which is no fun)

We strive for a clean, professional, and consistent codebase. Please ensure your code follows these rules:

### 1. Naming Conventions
*   **Functions (Public API):** Use `ogs` prefix with PascalCase (e.g., `ogsInit`, `ogsLoadBuffer`).
*   **Types:** PascalCase (e.g., `OgsContext`, `OgsBuffer`).
*   **Struct Members:** camelCase (e.g., `bufferHandle`, `splatCount`).
*   **Local Variables:** snake_case (e.g., `tmp_val`, `index_ptr`).
*   **Macros & Constants:** UPPER_SNAKE (e.g., `OGS_VERSION_MAJOR`).
*   **Internal Helpers:** Prefix with `_ogs` to denote private scope (e.g., `_ogsInternalInit`).

### 2. Brace Style
We use the K&R-inspired brace style. The opening brace must be on the same line as the function or statement.

**Correct:**
```c
void ogsRender(OgsContext* ctx) {
    if (ctx != NULL) {
        // Implementation
    }
}
```
**Incorrect:**
```c
void ogsRender(OgsContext* ctx) 
{
    // ...
}
```

### 3. Namespace Emulation

Since C does not have native namespaces, all public symbols must be prefixed with ogs. Keep the header files (include/ogs.h) clean and lean; avoid leaking Vulkan-specific types into the public API whenever possible.

Try to use private structs when possible to avoid exposing Vulkan types in the public API. For example:

**Correct:**
```c
// In header
typedef struct OgsContext OgsContext; // Forward declaration

OgsContext* ogsInit();

// In source

struct OgsContext {
    // Internal Vulkan handles and state
};
```
**Incorrect:**
```c
// In header
typedef struct {
    VkDevice device; // Exposes Vulkan type in public API
    // ...
} OgsContext;
```

## Project Structure
- `include/`: Public API headers ONLY!
- `src/`: Implementation files and private headers.
- `shaders/`: Shader source (CMake will compile these).
- `examples/`: Example applications demonstrating usage.
- `tests/`: Unit tests and validation code (hopefully these will exist at some point).
- `docs/`: Documentation and design notes.
- `CMakeLists.txt`: Build configuration.
- `README.md`: Project overview and quick start guide.
- `CONTRIBUTING.md`: Contribution guidelines (this file).
- `LICENSE`: License information.

## Workflow

1. Fork the repository.
2. Create a branch for your feature or bug fix.
3. Make your changes.
4. Ensure your code compiles with a CLEAN build with NO extra flags (following the cmake build instructions in the README).
5. Please try to avoid build warnings (my OCD will make me clean them up, but it is easier if you just fix them as you go).
6. Submit a pull request with a clear description of your changes.

## Technical Philosophy
- **Performance First:** OGS is written in pure C at the lowest level possible to maximize performance and minimize overhead.
- **Minimalism:** We avoid unnecessary abstractions and dependencies. The API is designed to be as simple and direct as possible, while still being powerful. Try to keep functions focused on a single responsibility. (Quote from my favorite professor: "If you have to use the word 'and' when describing what a function does, it should probably be more than one function.")
- **Cross-Platform:** While we are currently only testing one platform, we aim to keep the code portable and not use any platform-specific headers or APIs directly. We will use a custom cmake toolchain to allow us to create files like "some_files_win32.c" and "some_files_unix.c" and it will automatically compile the correct one based on the target platform. This will allow us to keep the code clean and avoid ifdefs everywhere.
- **Documentation:** We will strive to keep the code well-documented, both in terms of comments and external documentation. Please add comments to your code where necessary to explain complex logic or design decisions.

Thank you! I am a hobby developer working on this in my free time, along with 100s of other random side-projects, so I appreciate literally any help I can get, whether it's code contributions, testing on different platforms, or just feedback on the design and API.

#include <ogs.h>
#include <stdio.h>

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    printf("Initializing OpenGaussSplat v%d.%d.%d...\n", OGS_VERSION_MAJOR,
           OGS_VERSION_MINOR, OGS_VERSION_PATCH);

    OgsContext *ctx = ogsInit();
    if (!ctx) {
        fprintf(stderr, "Failed to initialize OGS context.\n");
        return 1;
    }

    printf("OGS initialized successfully.\n");

    ogsShutdown(ctx);
    printf("OGS shutdown complete.\n");
    return 0;
}

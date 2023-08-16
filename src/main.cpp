#include <openglContext.h>
#include <iostream>


int main()
{
    //=== INITIALIZATIONS ===//
    OpenglContext openglContext;
    openglContext.init();
    OpenglBuffersManager openglBuffersManager;

    srand(GLOBAL_SEED);

    //=== EXECUTION LOOP ===/
    openglContext.run(&openglBuffersManager);

    //=== EXIT ===/
    return EXIT_SUCCESS;
}

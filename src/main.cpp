#include <Simulator.h>
#include <iostream>


int main()
{
    
    //=== INITIALIZATIONS ===//
    simulator::Simulator Simulator;
    Simulator.init();
    OpenglBuffersManager openglBuffersManager;

    srand(GLOBAL_SEED);

    //=== EXECUTION LOOP ===/
    Simulator.run(&openglBuffersManager);

    //=== EXIT ===/
    return EXIT_SUCCESS;
}

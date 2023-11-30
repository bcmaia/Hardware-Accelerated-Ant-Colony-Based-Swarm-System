#include <Simulator.h>
#include <iostream>

#include "utils/math.hpp"
#include <cmath>
#include <functional>

int main()
{
    // std::function<float (float)> f = sinf;

    // auto rebound = [](float x, float s, float e) -> float {
    //     // Your code here to transform 'x' and return the result as a float.
    //     return s + math::mod(x - s, e - s); // Example: doubling the input value.
    // };

    // math::LookUpTable<float> mysin(f, 100, 0.0, 2.0 * M_PI, rebound);

    // std::cout << rebound(-1.0, ) << std::endl;

    // return 0;

    //=== INITIALIZATIONS ===//
    simulator::Simulator Simulator;
    Simulator.init();
    opengl::render::OpenglBuffersManager openglBuffersManager;

    srand(GLOBAL_SEED);

    //=== EXECUTION LOOP ===/
    Simulator.run(&openglBuffersManager);

    //=== EXIT ===/
    return EXIT_SUCCESS;
}

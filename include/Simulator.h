/**
 * @file Simulator.hpp
 * @brief This file contains the declaration of the Simulator class and
 * related structures.
 */

#pragma once

#include <iostream>

#include <environment.h>
#include <openglBuffersManager.h>

namespace simulator {
    using OpenglBuffersManager = opengl::render::OpenglBuffersManager;

    /**
     * @struct AdditionalCallbackParameters
     * @brief Additional parameters to be passed to GLFW callback functions.
     */
    struct AdditionalCallbackParameters {
        Camera *camera = nullptr;     ///< Camera object.
        GLFWwindow *window = nullptr; ///< GLFW window handle.
        UI *userInterface = nullptr;  ///< User interface object.
    };

    /**
     * @class Simulator
     * @brief Manages the OpenGL context and rendering loop.
     */
    class Simulator {
      public:
        Simulator(){}; ///< Constructor.
        ~Simulator();  ///< Destructor.

        void init(); ///< Initialize the OpenGL context.
        void run(OpenglBuffersManager *openglBuffersManager); 

      private:
        AdditionalCallbackParameters
            *AdditionalParameters; ///< Additional callback parameters.

        GLFWwindow
            *swarmSimulatorWindow; ///< GLFW window handle for the simulator.
        Camera *camera;            ///< Camera object.
        UI *userInterface;         ///< User interface object.
        swarm::Environment *environment;      ///< Environment object.
        ParameterAssigner *parameterAssigner; ///< Parameter assigner object.

        unsigned int frameCounter;       ///< Frame counter.
        int openGlRenderUpdateFrameRate; ///< Frame rate for OpenGL rendering
                                         ///< updates.

        void pollEvents(); ///< Poll GLFW events.

        void pre_render();  ///< Perform pre-render setup.
        void post_render(); ///< Perform post-render tasks.

        void handle_reset(OpenglBuffersManager *openglBuffersManager);
        void handle_running(OpenglBuffersManager *openglBuffersManager);
        void handle_paused(OpenglBuffersManager *openglBuffersManager);
    };
} // namespace simulator
/**
 * @file Simulator.cpp
 * @brief This file contains the implementation of the Simulator class
 * methods.
 */

#include <Simulator.h>

//========================================================================//
//=================// Static functions declarations //====================//
//========================================================================//

static void
keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

static void
cursorPositionCallback(GLFWwindow *window, double xpos, double ypos);

static void
mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);

//============================================================================//
//=================// Public Functions //=====================================//
//============================================================================//

// Destructor
simulator::Simulator::~Simulator() {
    glfwDestroyWindow(swarmSimulatorWindow);
    glfwTerminate();
}

/**
 * @brief Initializes the OpenGL context and necessary components.
 */
void simulator::Simulator::init() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    GLFWwindow *window = glfwCreateWindow(
        mode->width, mode->height, "Swarm Simulator", monitor, nullptr
    );
    glfwSetWindowMonitor(
        window, NULL, 1, 1, mode->width, mode->height, mode->refreshRate
    );

    swarmSimulatorWindow = window;

    camera = new Camera();
    camera->defMonitor();

    // glfw window creation
    // --------------------
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    userInterface = new UI();
    userInterface->init(swarmSimulatorWindow);

    openGlRenderUpdateFrameRate = 1;

    AdditionalParameters =
        (AdditionalCallbackParameters *)malloc(sizeof(AdditionalParameters));
    AdditionalParameters->camera = camera;
    AdditionalParameters->window = window;
    AdditionalParameters->userInterface = userInterface;

    glfwSetWindowUserPointer(
        window, reinterpret_cast<void *>(AdditionalParameters)
    );
}

/**
 * @brief Polls GLFW events and updates the frame counter.
 */
void simulator::Simulator::pollEvents() {
    glfwPollEvents();

    frameCounter++;
    frameCounter %= 1000;
}

/**
 * @brief Performs pre-render setup, including clearing buffers.
 */
void simulator::Simulator::pre_render() {
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 * @brief Performs post-render tasks, like swapping buffers.
 */
void simulator::Simulator::post_render() {
    glfwSwapBuffers(swarmSimulatorWindow);
}

/**
 * @brief Runs the rendering loop and manages different simulation states.
 * @param openglBuffersManager Pointer to the OpenGL buffers manager.
 */
void simulator::Simulator::run(OpenglBuffersManager *openglBuffersManager) {
    // Main rendering loop
    while (CLOSED != userInterface->stateSimulation &&
           !glfwWindowShouldClose(swarmSimulatorWindow)) {
        switch (userInterface->stateSimulation) {
        case RESET: {
            // Reset simulation state and buffers
            environment->resetEnvironment();
            openglBuffersManager->resetBufferManager();
            pre_render();

            // Loop until state changes
            while (RESET == userInterface->stateSimulation) {
                pollEvents(); // Increment frame counter here

                // Handle UI actions for environment initialization
                switch (userInterface->UIAction) {
                case ENVIRONMENT_INIT:
                    parameterAssigner =
                        new ParameterAssigner("experiments/experiment.json");
                    environment = new Environment(parameterAssigner);
                    environment->initializeEnvironment(openglBuffersManager);
                    environment->draw(openglBuffersManager, camera);
                    break; //
                case ADD_NEST:
                    environment->createNest(0, openglBuffersManager);
                    userInterface->UIAction = DO_NOTHING;
                    break; // case ADD_NEST

                case ADD_FOOD:
                    environment->createFoodSource(0, openglBuffersManager);
                    userInterface->UIAction = DO_NOTHING;
                    break; // case ADD_FOOD

                case ADD_ANT:
                    environment->createAnt(
                        userInterface->nestID, openglBuffersManager
                    );
                    userInterface->UIAction = DO_NOTHING;
                    break; // case ADD_ANT

                // TODO: think about what should happen here and implement
                // it.
                default:
                    throw std::runtime_error("Switch case not implemented");

                } // swtich

                userInterface->run();
                environment->draw(openglBuffersManager, camera);
                post_render();
            }    // while loo
        } break; // case RESET

        case RUNNING: {
            // Run simulation loop while in RUNNING state
            while (userInterface->stateSimulation == RUNNING) {
                pollEvents(); // Framecounter++ here

                environment->run(frameCounter);

                if (userInterface->turnOnGraphics &&
                    ((frameCounter % openGlRenderUpdateFrameRate) == 0)) {
                    openGlRenderUpdateFrameRate =
                        userInterface->openGlRenderUpdateFrameRate;
                    pre_render();
                    environment->draw(openglBuffersManager, camera);
                }                     // if statement
                userInterface->run(); // RETIRAR DAQUI PARA MAIOR
                                      // EXCLUSIVIDADE DO RUN
                post_render();
            } // while loop

        } break; // case RUNNING

        case PAUSED: {
            parameterAssigner = new ParameterAssigner(
                "src/swarmEnvironment/experiments/experiment.json"
            );
            environment = new Environment(parameterAssigner);
            environment->initializeEnvironment(openglBuffersManager);

            environment->createNest(0, openglBuffersManager);
            environment->createFoodSource(0, openglBuffersManager);
            environment->createFoodSource(1, openglBuffersManager);
            environment->createFoodSource(2, openglBuffersManager);
            environment->createFoodSource(3, openglBuffersManager);
            environment->createAnt(0, openglBuffersManager);
            while (userInterface->stateSimulation == PAUSED) {
                pollEvents();

                pre_render();
                environment->draw(openglBuffersManager, camera);

                userInterface->run();
                post_render();
            } // while loop

        } break; // case PAUSED

        case CLOSED:; // CLOSED wont happen

        } // switch statement
    }     // while loop
} // function scope

//============================================================================//
//=================// Static functions implementation /=/=====================//
//============================================================================//

using adparam = simulator::AdditionalCallbackParameters;

// process all input: query GLFW whether relevant keys are pressed/released
// this frame and react accordingly
static void
keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    adparam *AdditionalParameters =
        reinterpret_cast<adparam *>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        AdditionalParameters->userInterface->stateSimulation = CLOSED;

    if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS))
        AdditionalParameters->camera->ProcessKeyboard(UP);

    if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS))
        AdditionalParameters->camera->ProcessKeyboard(DOWN);

    if (key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS))
        AdditionalParameters->camera->ProcessKeyboard(LEFT);

    if (key == GLFW_KEY_D && (action == GLFW_REPEAT || action == GLFW_PRESS))
        AdditionalParameters->camera->ProcessKeyboard(RIGHT);
}

static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    adparam *AdditionalParameters =
        reinterpret_cast<adparam *>(glfwGetWindowUserPointer(window));

    AdditionalParameters->camera->ProcessMouseScroll(yoffset);
}

static void
cursorPositionCallback(GLFWwindow *window, double xpos, double ypos) {
    glfwGetCursorPos(window, &xpos, &ypos);
}

static void
mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    // if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
}

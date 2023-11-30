/**
 * @file openglBuffersManager.h
 * 
 * @brief This file contains the declaration of the OpenglBuffersManager class and related structs and functions.
 *
 * @ingroup Opengl
 * @ingroup Render
 * @ingroup BufferManagers
 * 
 */

#pragma once

#include <VAO.h>
#include <shader.h>

#include <UI.h>
#include <camera.h>

#include <ant.h>

// Pixel mapping (for pheromone)
extern int CHANNEL_COUNT; // Number of channels
extern GLenum PIXEL_FORMAT; // Format of pixel data
extern int DATA_SIZE;    // Size of the pixel data content
extern GLuint pboIds[2]; // IDs of PBO
extern GLuint textureId; // ID of texture
extern int indexPBO; // Index of the current PBO (Pixel Buffer Object)
extern int nextIndexPBO; // Index of the next PBO (Pixel Buffer Object)


namespace opengl {
    namespace render {
        /**
         * @brief Struct representing a vertex with color information.
         */
        struct ColorVertex {
            glm::vec3 position; /**< The position of the vertex. */
            glm::vec4 color; /**< The color of the vertex. */
        };

        /**
         * @brief Struct representing a vertex with texture information.
         */
        struct TextureVertex {
            glm::vec3 vertexPosition; /**< The position of the vertex. */
            glm::vec2 texturePosition; /**< The texture coordinates of the vertex. */
        };

        /**
         * @brief Class responsible for managing OpenGL buffers and rendering objects.
         */
        class OpenglBuffersManager {
          public:
            Shader *shaderAnts; /**< The shader for rendering ants. */
            Shader *shaderPheromone; /**< The shader for rendering pheromone. */

            vector<glm::mat4> foodsTransformationMatrices; /**< The transformation matrices for food objects. */
            VBO *foodsTransformationMatricesVBO; /**< The VBO for food transformation matrices. */
            VAO *foodsVAO; /**< The VAO for food objects. */

            vector<glm::mat4> anthillsTransformationMatrices; /**< The transformation matrices for anthill objects. */
            VBO *anthillsTransformationMatricesVBO; /**< The VBO for anthill transformation matrices. */
            VAO *anthillsVAO; /**< The VAO for anthill objects. */

            vector<glm::mat4> antsTransformationMatrices; /**< The transformation matrices for ant objects. */
            VBO *antsTransformationMatricesVBO; /**< The VBO for ant transformation matrices. */
            VAO *antsVAO; /**< The VAO for ant objects. */

            VAO *pheromoneVAO; /**< The VAO for pheromone objects. */
            GLbitfield *pixelMap; /**< The pixel map for storing pheromone data. */

            /**
             * @brief Default constructor for OpenglBuffersManager.
             */
            OpenglBuffersManager();

            /**
             * @brief Resets the buffer manager to its initial state.
             */
            void resetBufferManager();

            /**
             * @brief Adds an element to the specified transformation matrices vector.
             * @param transformationMatrices The transformation matrices vector.
             * @param size The size of the element.
             * @param theta The rotation angle of the element.
             * @param posX The X position of the element.
             * @param posY The Y position of the element.
             */
            void addElement(
                vector<glm::mat4> *transformationMatrices, float size,
                float theta, float posX, float posY
            );

            /**
             * @brief Updates the specified vertex buffer object with the given transformation matrices.
             * @param vertexBufferObject The vertex buffer object to update.
             * @param numberOfElements The number of elements in the transformation matrices vector.
             * @param transformationMatrices The transformation matrices vector.
             * @param usage The usage pattern of the vertex buffer object.
             */
            void updateBuffer(
                VBO *vertexBufferObject, int numberOfElements,
                vector<glm::mat4> transformationMatrices, GLenum usage
            );

            /**
             * @brief Updates the data of the specified vertex buffer object with the given transformation matrices.
             * @param vertexBufferObject The vertex buffer object to update.
             * @param numberOfElements The number of elements in the transformation matrices vector.
             * @param transformationMatrices The transformation matrices vector.
             */
            void updateBufferData(
                VBO *vertexBufferObject, int numberOfElements,
                vector<glm::mat4> transformationMatrices
            );

            /**
             * @brief Creates the components required for rendering food objects.
             */
            void createFoodComponents();

            /**
             * @brief Draws the specified number of food objects using the given camera.
             * @param numberOfFoods The number of food objects to draw.
             * @param camera The camera to use for rendering.
             */
            void drawFoods(int numberOfFoods, Camera *camera);

            /**
             * @brief Creates the components required for rendering anthill objects.
             */
            void createAnthillComponents();

            /**
             * @brief Draws the specified number of anthill objects using the given camera.
             * @param numberOfAnthills The number of anthill objects to draw.
             * @param camera The camera to use for rendering.
             */
            void drawAnthills(int numberOfAnthills, Camera *camera);

            /**
             * @brief Creates the components required for rendering ant objects.
             */
            void createAntComponents();

            /**
             * @brief Draws the specified number of ant objects using the given camera.
             * @param numberOfAnts The number of ant objects to draw.
             * @param camera The camera to use for rendering.
             */
            void drawAnts(int numberOfAnts, Camera *camera);

            /**
             * @brief Updates the model of the specified number of ant objects with the given ants vector.
             * @param numberOfAnts The number of ant objects to update.
             * @param ants The vector of ant objects.
             */
            void updateModelAnts(int numberOfAnts, vector<swarm::Ant *> ants);

            /**
             * @brief Creates the components required for rendering pheromone objects.
             */
            void createPheromoneComponents();

            /**
             * @brief Creates the texture buffer for storing pheromone data.
             */
            void createTextureBuffer();

            /**
             * @brief Creates the pixel buffers for storing pheromone data.
             */
            void createPixelBuffers();

            /**
             * @brief Swaps the pixel buffers and updates the pheromone matrix.
             * @param pheromoneMatrix The pheromone matrix to update.
             */
            void swapPixelBuffers(uint8_t *pheromoneMatrix);

            /**
             * @brief Draws the pheromone objects using the given pheromone matrix and camera.
             * @param pheromoneMatrix The pheromone matrix to use for rendering.
             * @param camera The camera to use for rendering.
             */
            void drawPheromone(uint8_t *pheromoneMatrix, Camera *camera);
        };
    } // namespace render
} // namespace opengl
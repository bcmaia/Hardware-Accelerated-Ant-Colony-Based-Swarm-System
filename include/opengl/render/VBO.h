/**
 * @file VBO.h
 * 
 * @brief This file contains the declaration of the VBO class, which represents
 * a Vertex Buffer Object in OpenGL.
 * 
 * @ingroup Opengl
 * @ingroup Render
 * 
 */

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace opengl {
    namespace render {
        /**
         * @brief The VBO class represents a Vertex Buffer Object in OpenGL.
         * 
         * A Vertex Buffer Object (VBO) is a buffer that stores vertex data on the GPU.
         * This class provides methods to create, resize, update, bind, unbind, and erase
         * a VBO.
         */
        class VBO {
            public:
                // Reference ID of the Vertex Buffer Object
                GLuint ID;

                /**
                 * @brief Constructs a VBO with the specified size, data, and usage.
                 * 
                 * @param size The size of the buffer in bytes.
                 * @param data A pointer to the data to be stored in the buffer.
                 * @param usage The usage pattern of the buffer (e.g., GL_STATIC_DRAW, GL_DYNAMIC_DRAW).
                 */
                VBO(GLsizeiptr size, void *data, GLenum usage);

                /**
                 * @brief Resizes the VBO with the specified size, data, and usage.
                 * 
                 * @param size The new size of the buffer in bytes.
                 * @param data A pointer to the new data to be stored in the buffer.
                 * @param usage The new usage pattern of the buffer (e.g., GL_STATIC_DRAW, GL_DYNAMIC_DRAW).
                 */
                void resizeBuffer(GLsizeiptr size, void *data, GLenum usage);

                /**
                 * @brief Updates a portion of the VBO with the specified size and data.
                 * 
                 * @param size The size of the data to be updated in bytes.
                 * @param data A pointer to the new data to be stored in the buffer.
                 */
                void subBufferData(GLsizeiptr size, void *data);

                /**
                 * @brief Binds the VBO to the current OpenGL context.
                 */
                void bind();

                /**
                 * @brief Unbinds the VBO from the current OpenGL context.
                 */
                void unbind();

                /**
                 * @brief Erases the VBO from the GPU memory.
                 */
                void erase();
        }; // class VBO
    } // namespace render
} // namespace opengl
/** 
 * @file VAO.h
 * 
 * @brief This file contains the declaration of the VAO class, which represents
 * a Vertex Array Object in OpenGL.
 * 
 * @ingroup Opengl
 * @ingroup Render
 * 
 */

#pragma once

#include <string>

#include <opengl/render/EBO.h>
#include <opengl/render/VBO.h>

namespace opengl {
    namespace render {
        class VAO {
          public:
            // ID reference for the Vertex Array Object
            GLuint ID;
            // Constructor that generates a VAO ID
            VAO();

            // Links a VBO to the VAO using a certain layout
            void linkVBO(
                VBO &VBO, GLuint layout, GLuint size, GLenum type,
                GLsizei stride, GLvoid *offset
            );
            void linkVBOMatrix4(
                VBO &VBO, GLuint layout, GLuint size, GLenum type,
                GLsizei stride
            );

            void bind();
            void unbind();
            void erase();
        };

    } // namespace render
} // namespace opengl
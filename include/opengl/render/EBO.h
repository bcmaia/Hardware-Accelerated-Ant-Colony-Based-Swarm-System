/**
 * @file EBO.h
 * 
 * @brief This file contains the declaration of the EBO class, which represents
 * 
 * @ingroup Opengl
 * @ingroup Render
 *
 */

#pragma once
#include <glad/glad.h>

namespace opengl {
    namespace render {
        /**
         * @brief The EBO class represents an Elements Buffer Object in OpenGL.
         * 
         * An Elements Buffer Object (EBO) is used to store index data that specifies the order in which vertices should be rendered.
         * This class provides methods to bind, unbind, and erase the EBO.
         */
        class EBO {
            public:
                GLuint ID; /**< ID reference of Elements Buffer Object */

                /**
                 * @brief Constructs an EBO and links it to the given indices.
                 * 
                 * @param size The size of the indices array.
                 * @param indices A pointer to the indices array.
                 */
                EBO(GLsizeiptr size, GLuint *indices);

                /**
                 * @brief Binds the EBO.
                 */
                void bind();

                /**
                 * @brief Unbinds the EBO.
                 */
                void unbind();

                /**
                 * @brief Erases the EBO.
                 */
                void erase();
        }; // class EBO
    } // namespace render
} // namespace opengl

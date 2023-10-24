#pragma once
#include <glad/glad.h>

namespace opengl {
    namespace render {
        class EBO {
          public:
            // ID reference of Elements Buffer Object

            GLuint ID;
            // Constructor that generates a Elements Buffer Object and links it
            // to indices
            EBO(GLsizeiptr size, GLuint *indices);

            void bind();
            void unbind();
            void erase();
        };
    } // namespace render
} // namespace opengl

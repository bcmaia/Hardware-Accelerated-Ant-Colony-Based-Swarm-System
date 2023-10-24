#pragma once

#include <../dep/glad/glad.h>
#include <glm/glm.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>

namespace opengl {
    namespace render {
        class Shader {
          public:
            Shader();

            std::string extractText(const char *filePath);
            GLuint loadShader(GLenum type, const char *filePath);
            void createProgramLinkShaders(
                const char *vertexPath, const char *fragmentPath,
                const char *geometryPath, const char *pixelPath
            );
            void bind() const;
            void unBind() const;
            GLuint getProgramID() const;
            void dispose();

            void setMat4(const std::string &name, const glm::mat4 &mat) const;

            void checkErrors(GLuint shaderObj, std::string type);

          private:
            GLuint programID;
            GLuint vertexShader, fragmentShader, geometryShader;
        };
    } // namespace render
} // namespace opengl
/**
 * @file shader.h
 * @brief Shader class header file.
 *
 * This file contains the Shader class, which is used to load and compile
 * shaders.
 *
 * @see https://learnopengl.com/Getting-started/Shaders
 *
 * @ingroup Opengl
 * @ingroup Render
 * 
 */

#pragma once

#include <../dep/glad/glad.h>
#include <glm/glm.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>

namespace opengl {
    namespace render {
        /**
         * @brief The Shader class represents an OpenGL shader program.
         */
        class Shader {
          public:
            /**
             * @brief Constructs a Shader object.
             */
            Shader();

            /**
             * @brief Extracts the text content from a file.
             * @param filePath The path to the file.
             * @return The text content of the file.
             */
            std::string extractText(const char *filePath);

            /**
             * @brief Loads and compiles a shader from a file.
             * @param type The type of the shader (e.g., GL_VERTEX_SHADER, GL_FRAGMENT_SHADER).
             * @param filePath The path to the shader file.
             * @return The ID of the loaded shader.
             */
            GLuint loadShader(GLenum type, const char *filePath);

            /**
             * @brief Creates and links a shader program from vertex, fragment, geometry, and pixel shaders.
             * @param vertexPath The path to the vertex shader file.
             * @param fragmentPath The path to the fragment shader file.
             * @param geometryPath The path to the geometry shader file.
             * @param pixelPath The path to the pixel shader file.
             */
            void createProgramLinkShaders(
                const char *vertexPath, const char *fragmentPath,
                const char *geometryPath, const char *pixelPath
            );

            /**
             * @brief Binds the shader program for rendering.
             */
            void bind() const;

            /**
             * @brief Unbinds the shader program.
             */
            void unBind() const;

            /**
             * @brief Returns the ID of the shader program.
             * @return The ID of the shader program.
             */
            GLuint getProgramID() const;

            /**
             * @brief Disposes the shader program and releases associated resources.
             */
            void dispose();

            /**
             * @brief Sets a 4x4 matrix uniform in the shader program.
             * @param name The name of the uniform variable.
             * @param mat The 4x4 matrix to set.
             */
            void setMat4(const std::string &name, const glm::mat4 &mat) const;

            /**
             * @brief Checks for compilation or linking errors in the shader object.
             * @param shaderObj The ID of the shader object to check.
             * @param type The type of the shader object (e.g., "VERTEX", "FRAGMENT").
             */
            void checkErrors(GLuint shaderObj, std::string type);

          private:
            GLuint programID; ///< The ID of the shader program.
            GLuint vertexShader; ///< The ID of the vertex shader.
            GLuint fragmentShader; ///< The ID of the fragment shader.
            GLuint geometryShader; ///< The ID of the geometry shader.
        };
    } // namespace render
} // namespace opengl
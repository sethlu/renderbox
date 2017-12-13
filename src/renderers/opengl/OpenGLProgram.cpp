#include "platform.h"
#if defined(__MACOSX__)
#include <OpenGL/OpenGL.h>
#elif defined(__IPHONEOS__)
#include <OpenGLES/ES3/gl.h>
#endif
#include <string>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "OpenGLProgram.h"
#include "GLSLPreprocessor.h"


namespace renderbox {

    std::unordered_map<GLuint, OpenGLProgram *> OpenGLProgram::programs;

    OpenGLProgram *OpenGLProgram::getProgram(GLuint programId) {
        auto it = programs.find(programId);
        return it != programs.end() ? it->second : nullptr;
    }

    OpenGLProgram::OpenGLProgram(const char *vertexShaderSource, const char *fragmentShaderSource)
        : pointLights(false), materialColor(false), worldMatrix(false), sceneAmbientColor(false),
          worldNormalMatrix(false), numActivePointLights(false), worldProjectionMatrix(false) {

        programId = glCreateProgram();
        programs[programId] = this;

        OpenGLShader vertexShader(vertexShaderSource, GL_VERTEX_SHADER);
        OpenGLShader fragmentShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

        // Link shader
        glAttachShader(programId, vertexShader.shaderId);
        glAttachShader(programId, fragmentShader.shaderId);
        glLinkProgram(programId);
        glDetachShader(programId, vertexShader.shaderId);
        glDetachShader(programId, fragmentShader.shaderId);

        // Check if linked
        GLint linked;
        glGetProgramiv(programId, GL_LINK_STATUS, &linked);
        if (!linked) {
            GLint length;
            glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);

            auto log = new GLchar[length + 1];
            glGetProgramInfoLog(programId, length, &length, log);
            fprintf(stderr, "Linking error: %s\n", log);

            throw 2;
        }

        // Get uniform names
        GLint uniforms;
        GLsizei uniformNameBufferSize;
        glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &uniforms);
        glGetProgramiv(programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformNameBufferSize);

        GLsizei uniformNameSize;
        GLchar uniformName[uniformNameBufferSize];
        for (GLuint uniformIndex = 0; uniformIndex < uniforms; ++uniformIndex) {
            glGetActiveUniformName(programId, uniformIndex, uniformNameBufferSize, &uniformNameSize, uniformName);

            std::cout << uniformName << std::endl;

            // Assign bool flags for "built-in" uniforms
            if (uniformNameSize < 4) continue;

            // Find the index of `[` or fallback to 0
            unsigned int array = static_cast<unsigned int>(uniformNameSize - 3);
            while (array > 0) {
                if (uniformName[array] == '[') break;
                --array;
            }

#define HASH(LEN, FIRST, FOURTH) \
    (((LEN) << 5) + ((((FIRST) - 'a') + ((FOURTH) - 'a')) & 31))
#define CASE(LEN, FIRST, NAME) \
    case HASH((LEN) + 3, 'r', FIRST): \
        if (memcmp(uniformName, "rb_"#NAME, LEN) == 0) { (NAME) = true; break; }

            if (array) {
                // Only match array name

                switch (HASH(array, uniformName[0], uniformName[3])) {

                    CASE(11, 'p', pointLights);

                    default: break;
                }

                continue;
            }

            switch (HASH(uniformNameSize, uniformName[0], uniformName[3])) {

                CASE(11, 'w', worldMatrix);

                CASE(13, 'm', materialColor);

                CASE(17, 's', sceneAmbientColor);
                CASE(17, 'w', worldNormalMatrix);

                CASE(20, 'n', numActivePointLights);

                CASE(21, 'w', worldProjectionMatrix);

                default: break;
            }

#undef CASE
#undef HASH

        }

    }

    OpenGLProgram::OpenGLProgram(std::string &vertexShaderSource, std::string &fragmentShaderSource)
        : OpenGLProgram(vertexShaderSource.c_str(), fragmentShaderSource.c_str()) {

    }

    OpenGLProgram *OpenGLProgram::createProgramWithPreprocessedSources(const char *vertexShaderSource,
                                                                       const char *fragmentShaderSource,
                                                                       const std::string &bootstrap) {
        std::string vertexShaderSource_ = preprocessGLSLSource(vertexShaderSource, bootstrap),
                    fragmentShaderSource_ = preprocessGLSLSource(fragmentShaderSource, bootstrap);
        return new OpenGLProgram(vertexShaderSource_, fragmentShaderSource_);
    }

    OpenGLProgram *OpenGLProgram::createProgramWithSourceFiles(const char *vertexShaderFilename,
                                                               const char *fragmentShaderFilename) {
        std::string vertexShaderSource = readFile(vertexShaderFilename),
                    fragmentShaderSource = readFile(fragmentShaderFilename);
        return new OpenGLProgram(vertexShaderSource, fragmentShaderSource);
    }

    OpenGLProgram *OpenGLProgram::createProgramWithPreprocessedSourceFiles(const char *vertexShaderFilename,
                                                                           const char *fragmentShaderFilename,
                                                                           const std::string &bootstrap) {
        std::string vertexShaderSource = preprocessGLSLSourceFile(vertexShaderFilename, bootstrap),
                    fragmentShaderSource = preprocessGLSLSourceFile(fragmentShaderFilename, bootstrap);
        return new OpenGLProgram(vertexShaderSource, fragmentShaderSource);
    }

    GLuint OpenGLProgram::getProgramId() const {
        return programId;
    }

    inline void OpenGLProgram::useProgram(GLuint programId) {
        glUseProgram(programId);
    }

    void OpenGLProgram::useProgram() {
        useProgram(programId);
    }

    void OpenGLProgram::stopProgram() {
        glUseProgram(0);
    }

    GLint OpenGLProgram::getAttributeLocation(const char *name) {
        return glGetAttribLocation(programId, name);
    }

    GLint OpenGLProgram::getUniformLocation(const char *name) {
        return glGetUniformLocation(programId, name);
    }

    OpenGLShader::OpenGLShader(const char *source, GLenum type) {

        shaderId = glCreateShader(type);

        std::cout << "OPENGL_SHADER" << std::endl
                  << source
                  << "END OPENGL SHADER" << std::endl;

        // Compile shader
        glShaderSource(shaderId, 1, &source, nullptr);
        glCompileShader(shaderId);

        // Check if compiled
        GLint compiled;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint length;
            glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);

            auto log = new GLchar[length + 1];
            glGetShaderInfoLog(shaderId, length, &length, log);
            fprintf(stderr, "Compilation error: %s\n", log);

            throw 2;
        }

    }

    OpenGLShader::~OpenGLShader() {

        glDeleteShader(shaderId);

    }

}
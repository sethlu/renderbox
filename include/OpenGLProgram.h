#ifndef RENDERBOX_OPENGLPROGRAM_H
#define RENDERBOX_OPENGLPROGRAM_H


#include <unordered_map>
#include <string>

#include "OpenGL.h"
#include "OpenGLBuffer.h"


namespace renderbox {

    class OpenGLProgram {

        friend class OpenGLRenderer;

    public:

        OpenGLProgram(const char *vertexShaderSource,
                      const char *fragmentShaderSource);

        OpenGLProgram(std::string &vertexShaderSource,
                      std::string &fragmentShaderSource);

        static OpenGLProgram *createProgramWithPreprocessedSources(const char *vertexShaderSource,
                                                                   const char *fragmentShaderSource,
                                                                   const std::string &bootstrap = "");

        static OpenGLProgram *createProgramWithSourceFiles(const char *vertexShaderFilename,
                                                           const char *fragmentShaderFilename);

        static OpenGLProgram *createProgramWithPreprocessedSourceFiles(const char *vertexShaderFilename,
                                                                       const char *fragmentShaderFilename,
                                                                       const std::string &bootstrap = "");

        static OpenGLProgram *getProgram(GLuint programId);

        inline GLuint getProgramId() const {
            return programId;
        }

        static inline void useProgram(GLuint programId) {
            glUseProgram(programId);
        }

        inline void useProgram() const {
            useProgram(programId);
        }

        inline static void stopProgram() {
            glUseProgram(0);
        }

        inline GLint getAttributeLocation(const char *name) const {
            return glGetAttribLocation(programId, name);
        };

        inline GLint getUniformLocation(const char *name) const {
            return glGetUniformLocation(programId, name);
        };

    protected:

        static std::unordered_map<GLuint, OpenGLProgram *> programs;

        GLuint programId;

        // Uniform locations

        // Corresponding to lights_preamble.glsl
        struct PointLight {
            GLint position;
            GLint color;
        };

        std::vector<struct PointLight> pointLights;
        GLint worldMatrix;
        GLint sceneAmbientColor;
        GLint worldNormalMatrix;
        GLint materialAmbientColor;
        GLint materialAmbientMap;
        GLint materialDiffuseColor;
        GLint materialDiffuseMap;
        GLint numActivePointLights;
        GLint worldProjectionMatrix;

    };

    class OpenGLShader {

        friend class OpenGLProgram;

    public:

        OpenGLShader(const char *source, GLenum type);

        ~OpenGLShader();

    protected:

        GLuint shaderId;

    };

}


#endif //RENDERBOX_OPENGLPROGRAM_H

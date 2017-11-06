#include "platform.h"
#if defined(__MACOSX__)
#include <OpenGL/OpenGL.h>
#elif defined(__IPHONEOS__)
#include <OpenGLES/ES3/gl.h>
#endif
#include <string>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include "OpenGLProgram.h"
#include "Material.h"


namespace renderbox {

    std::unordered_map<GLuint, OpenGLProgram *> OpenGLProgram::programs;

    OpenGLProgram *OpenGLProgram::getProgram(GLuint programID) {
        auto it = programs.find(programID);
        return it != programs.end() ? it->second : nullptr;
    }

    GLuint initShader(const GLchar *source, GLenum shader_type) {
        // Create shaders
        GLuint shader = glCreateShader(shader_type);

        // Compile shaders
        GLint compiled;
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (compiled) {
            return shader;
        }

        // Compilation encountered error
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        GLchar *log = new GLchar[length + 1];
        glGetShaderInfoLog(shader, length, &length, log);
        fprintf(stderr, "Compilation error: %s\n", log);
        throw 3; // TODO: Find error no
    }

    GLuint initProgram(GLuint vertexShaderID, GLuint fragmentShaderID) {
        // Create program
        GLuint programID = glCreateProgram();

        // Link program
        GLint linked;
        glAttachShader(programID, vertexShaderID);
        glAttachShader(programID, fragmentShaderID);
        glLinkProgram(programID);
        glGetProgramiv(programID, GL_LINK_STATUS, &linked);
        if (linked) {
            return programID;
        }

        // Linking encountered error
        GLint length;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length);
        GLchar *log = new GLchar[length + 1];
        glGetProgramInfoLog(programID, length, &length, log);
        fprintf(stderr, "Linking error: %s\n", log);
        throw 4; // TODO: Find error no
    }

    GLuint initProgram(const char *vertexShaderSource, const char *fragmentShaderSource) {
        GLuint vertexShaderID = initShader(vertexShaderSource, GL_VERTEX_SHADER);
        GLuint fragmentShaderID = initShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

        GLuint programID = initProgram(vertexShaderID, fragmentShaderID);

        glDetachShader(programID, vertexShaderID);
        glDetachShader(programID, fragmentShaderID);

        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);

        return programID;
    }

    OpenGLProgram::OpenGLProgram(const char *vertexShaderSource, const char *fragmentShaderSource) {
        programId = initProgram(vertexShaderSource, fragmentShaderSource);
        programs[programId] = this;
    }

    OpenGLProgram::OpenGLProgram(std::string vertexShaderSource, std::string fragmentShaderSource) {
        OpenGLProgram(copyString(vertexShaderSource), copyString(fragmentShaderSource));
    }

    GLuint OpenGLProgram::getProgramId() const {
        return programId;
    }

    inline void OpenGLProgram::useProgram(GLuint programID) {
        glUseProgram(programID);
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

    std::string readFile(const char *name) {
        std::ifstream in;
        std::string line, str;
        in.open(name);
        if (in.is_open()) {
            while (!in.eof()) {
                getline(in, line);
                str += line + "\n";
            }
            return str;
        }
        fprintf(stdout, "Failed to open file: %s\n", name);
        throw 2; // TODO: Find error no
    }


}
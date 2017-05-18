#include <GL/glew.h>
#include <string>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include "OpenGLProgram.h"


namespace renderbox {

    std::string readFile(const char *name) {
        std::ifstream in;
        std::string line, str = "";
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

    GLuint initShader(const char *name, GLenum shader_type) {
        // Create shaders
        GLuint shader = glCreateShader(shader_type);

        // Read shaders file
        std::string str = readFile(name);
        GLchar *cstr = new GLchar[str.length() + 1]; // Require space for terminating null character
        strcpy(cstr, str.c_str());
        const GLchar *constcstr = cstr;

        // Compile shaders
        GLint compiled;
        glShaderSource(shader, 1, &constcstr, nullptr);
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

    GLuint initProgram(const char *vertexShaderName, const char *fragmentShaderName) {
        GLuint vertexShaderID = initShader(vertexShaderName, GL_VERTEX_SHADER);
        GLuint fragmentShaderID = initShader(fragmentShaderName, GL_FRAGMENT_SHADER);

        GLuint programID = initProgram(vertexShaderID, fragmentShaderID);

        glDetachShader(programID, vertexShaderID);
        glDetachShader(programID, fragmentShaderID);

        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);

        return programID;
    }

    OpenGLProgram::OpenGLProgram(const char *vertexShaderName, const char *fragmentShaderName) {

        GLuint vertexShaderID = initShader(vertexShaderName, GL_VERTEX_SHADER);
        GLuint fragmentShaderID = initShader(fragmentShaderName, GL_FRAGMENT_SHADER);

        programID = initProgram(vertexShaderID, fragmentShaderID);

        glDetachShader(programID, vertexShaderID);
        glDetachShader(programID, fragmentShaderID);

        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);

    }

    GLuint OpenGLProgram::getProgramID() {
        return programID;
    }

    void OpenGLProgram::useProgram() {
        glUseProgram(programID);
    }

    void OpenGLProgram::stopProgram() {
        glUseProgram(0);
    }

    void OpenGLProgram::setUniform(const char *name, glm::mat4x4 &matrix) {
        useProgram();

        GLint location = glGetUniformLocation(programID, name);
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));

        stopProgram();
    }

    GLint OpenGLProgram::getAttributeLocation(const char *name) {
        return glGetAttribLocation(programID, name);
    }


}
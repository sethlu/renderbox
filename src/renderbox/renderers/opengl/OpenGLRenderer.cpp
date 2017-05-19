#include <iostream>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include "OpenGLRenderer.h"
#include "../../objects/Mesh.h"
#include "OpenGLProgram.h"


namespace renderbox {

    void Renderer::loadObject(Object *object) {

        // Needs to load the buffer when the vertices are not yet loaded or when vertices have been updated

        OpenGLObjectProperties *objectProperties = properties->getObjectProperties(object);
        OpenGLVertexArray *vertexArray = objectProperties->getVertexArray(0);
        OpenGLProgram *program = objectProperties->getProgram(object->getMaterial());

        // Load object geometry

        std::vector<glm::vec3> vertices = object->getGeometry()->getVertices();
        std::vector<glm::uvec3> faces = object->getGeometry()->getFaces();

        unsigned long nVertices = vertices.size();
        GLfloat bufferVertices[nVertices * 3];
        for (unsigned long i = 0; i < nVertices; ++i) {
            glm::vec3 vertex = vertices[i];
            bufferVertices[3 * i] = vertex[0];
            bufferVertices[3 * i + 1] = vertex[1];
            bufferVertices[3 * i + 2] = vertex[2];
        }

        objectProperties->getBuffer(0)->buffer(bufferVertices, sizeof(bufferVertices));
        vertexArray->enableAttribute(program, "vertexPosition");
        vertexArray->setAttributeBuffer(program, "vertexPosition", objectProperties->getBuffer(0));

        unsigned long nFaces = faces.size();
        GLuint bufferFaces[nFaces * 3];
        for (unsigned long i = 0; i < nFaces; ++i) {
            glm::uvec3 face = faces[i];
            bufferFaces[3 * i] = face[0];
            bufferFaces[3 * i + 1] = face[1];
            bufferFaces[3 * i + 2] = face[2];
        }

        objectProperties->getBuffer(1)->buffer(bufferFaces, sizeof(bufferFaces));
        vertexArray->setElementBuffer(objectProperties->getBuffer(1));

    }

    void Renderer::renderObject(Object *object, Camera *camera, glm::mat4x4 viewProjectionMatrix) {

        if (!object->visible) return;

        if (object->hasGeometry() && object->hasMaterial()) {

            glm::mat4x4 worldProjectionMatrix = viewProjectionMatrix * object->getWorldMatrix();

            OpenGLObjectProperties *objectProperties = properties->getObjectProperties(object);
            OpenGLVertexArray *vertexArray = objectProperties->getVertexArray(0);
            OpenGLProgram *program = objectProperties->getProgram(object->getMaterial());

            // Set world-view-projection matrix
            program->setUniform("worldProjectionMatrix", worldProjectionMatrix);

            // Use program and vertex array for drawing
            program->useProgram();
            vertexArray->bindVertexArray();

            // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            // glDrawArrays(GL_TRIANGLES, 0, (GLsizei) object->getGeometry()->getFaces().size() * 3);
            glDrawElements(GL_TRIANGLES, object->getGeometry()->getFaces().size() * 3, GL_UNSIGNED_INT, 0);

            vertexArray->unbindVertexArray();

        }

        for (Object *child : object->getChildren()) {
            renderObject(child, camera, viewProjectionMatrix);
        }

    }

    void Renderer::renderObject(Object *object, Camera *camera) {
        renderObject(object, camera, camera->getViewProjectionMatrix());
    }

    void Renderer::render(Scene *scene, Camera *camera, OpenGLRenderTarget *renderTarget, bool forceClear) {

        // Use frame buffer from render target
        glBindFramebuffer(GL_FRAMEBUFFER, renderTarget ? renderTarget->getFramebufferID() : 0);

        if (renderTarget) {
            glViewport(0, 0, renderTarget->getWidth(), renderTarget->getHeight());
        } else {
            glViewport(0, 0, getFramebufferWidth(), getFramebufferHeight());
        }

        // Clear the scene
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderObject(scene, camera);

    }

    Renderer::Renderer() {

        // GLFW

        if (!glfwInit()) {
            fprintf(stderr, "Failed to initialize GLFW\n");
            throw 2;
        }

        glfwWindowHint(GLFW_SAMPLES, 4); // Anti-aliasing
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create a window
        window = glfwCreateWindow(800, 600, "Particle Sim", nullptr, nullptr);
        if (!window) {

            fprintf(stderr, "Failed to open GLFW window\n");

            // Terminate GLFW
            glfwTerminate();
            throw 2;

        }
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

        // GLEW

        glewExperimental = GL_TRUE;
        GLenum err = glewInit();
        if (err != GLEW_OK) {
            fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
            throw 2;
        }
        fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

        // Enable depth tests
        glEnable(GL_DEPTH_TEST);

    }

    Renderer::~Renderer() {
        glfwDestroyWindow(window);
    }

    GLFWwindow *Renderer::getWindow() {
        return window;
    }

    int Renderer::getWindowWidth() {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        return width;
    }

    int Renderer::getWindowHeight() {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        return height;
    }

    int Renderer::getFramebufferWidth() {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        return width;
    }

    int Renderer::getFramebufferHeight() {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        return height;
    }

}
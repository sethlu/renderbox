#include <iostream>
#include <queue>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include "OpenGLRenderer.h"
#include "../../renderbox.h"


namespace renderbox {

    void OpenGLRenderer::loadObject(Object *object) {

        // Needs to load the buffer when the vertices are not yet loaded or when vertices have been updated

        OpenGLObjectProperties *objectProperties = properties->getObjectProperties(object);
        OpenGLVertexArray *vertexArray = objectProperties->getVertexArray(0);
        OpenGLProgram *program = properties->getProgram(object->getMaterial());

        // Load object geometry

        std::vector<glm::vec3> vertices = object->getGeometry()->getVertices();
        std::vector<glm::vec3> normals = object->getGeometry()->getNormals();
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
        vertexArray->setAttributeBuffer(program, "vertexPosition", objectProperties->getBuffer(0));
        vertexArray->enableAttribute(program, "vertexPosition");

        unsigned long nNormals = normals.size();
        GLfloat bufferNormals[nNormals * 3];
        for (unsigned long i = 0; i < nNormals; ++i) {
            glm::vec3 normal = normals[i];
            bufferNormals[3 * i] = normal[0];
            bufferNormals[3 * i + 1] = normal[1];
            bufferNormals[3 * i + 2] = normal[2];
        }

        objectProperties->getBuffer(1)->buffer(bufferNormals, sizeof(bufferNormals));
        vertexArray->setAttributeBuffer(program, "vertexNormal", objectProperties->getBuffer(1));
        vertexArray->enableAttribute(program, "vertexNormal");

        unsigned long nFaces = faces.size();
        GLuint bufferFaces[nFaces * 3];
        for (unsigned long i = 0; i < nFaces; ++i) {
            glm::uvec3 face = faces[i];
            bufferFaces[3 * i] = face[0];
            bufferFaces[3 * i + 1] = face[1];
            bufferFaces[3 * i + 2] = face[2];
        }

        objectProperties->getBuffer(2)->buffer(bufferFaces, sizeof(bufferFaces));
        vertexArray->setElementBuffer(objectProperties->getBuffer(2));

    }

    OpenGLRenderList *OpenGLRenderer::prepassRender(Object *object, Camera *camera) {
        OpenGLRenderList *renderList = new OpenGLRenderList;

        std::queue<Object *> frontier;
        frontier.push(object);

        while (!frontier.empty()) {
            Object *current = frontier.front();
            frontier.pop();

            // Skip invisible objects
            if (!current->visible) continue;

            // Do not add objects without geometry or material
            if (current->hasGeometry() && current->hasMaterial()) {
                OpenGLProgram *program = properties->getProgram(current->getMaterial());
                renderList->addObject(program->getProgramID(), current);
            }

            for (Object *next : current->getChildren()) {
                frontier.push(next);
            }
        }

        return renderList;
    }

    void OpenGLRenderer::render(OpenGLRenderList *renderList, Camera *camera, OpenGLRenderTarget *renderTarget, bool forceClear) {

        //
        // Prepare
        //

        // Use frame buffer from render target
        glBindFramebuffer(GL_FRAMEBUFFER, renderTarget ? renderTarget->getFramebufferID() : 0);

        if (renderTarget) {
            glViewport(0, 0, renderTarget->getWidth(), renderTarget->getHeight());
        } else {
            glViewport(0, 0, getFramebufferWidth(), getFramebufferHeight());
        }

        // Clear the scene
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //
        // Render
        //

        // View projection matrix
        glm::mat4x4 viewProjectionMatrix = camera->getViewProjectionMatrix();

        for (auto it = renderList->objects.begin(); it != renderList->objects.end(); ++it) {

            // Use program
            OpenGLProgram *program = OpenGLProgram::getProgram(it->first);

            for (Object *object : it->second) {

                OpenGLObjectProperties *objectProperties = properties->getObjectProperties(object);
                OpenGLVertexArray *vertexArray = objectProperties->getVertexArray(0);
                Material *material = object->getMaterial();

                // World projection matrix
                glm::mat4x4 worldProjectionMatrix = viewProjectionMatrix * object->getWorldMatrix();

                // Set common uniforms
                glUniformMatrix4fv(program->getUniformLocation("worldProjectionMatrix"),
                                   1,
                                   GL_FALSE,
                                   glm::value_ptr(worldProjectionMatrix));
                if (material->getMaterialType() == MESH_BASIC_MATERIAL
                        || material->getMaterialType() == MESH_LAMBERT_MATERIAL) {
                    glUniform3fv(program->getUniformLocation("vertexColor"),
                                 1,
                                 glm::value_ptr(((MeshBasicMaterial *) material)->color));
                }
                if (material->getMaterialType() == MESH_LAMBERT_MATERIAL) {
                    glUniformMatrix4fv(program->getUniformLocation("worldMatrix"),
                                       1,
                                       GL_FALSE,
                                       glm::value_ptr(object->getWorldMatrix()));
                    glUniformMatrix4fv(program->getUniformLocation("worldNormalMatrix"),
                                       1,
                                       GL_FALSE,
                                       glm::value_ptr(glm::transpose(glm::inverse(object->getWorldMatrix()))));
                }

                // Bind vertex array
                program->useProgram();
                vertexArray->bindVertexArray();

                // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                // glDrawArrays(GL_TRIANGLES, 0, (GLsizei) object->getGeometry()->getFaces().size() * 3);
                glDrawElements(GL_TRIANGLES, object->getGeometry()->getFaces().size() * 3, GL_UNSIGNED_INT, 0);

            }
        }

        // Unbind vertex array
        OpenGLVertexArray::unbindVertexArray();

    }

    void OpenGLRenderer::render(Scene *scene, Camera *camera, OpenGLRenderTarget *renderTarget, bool forceClear) {
        render(prepassRender(scene, camera),
               camera, renderTarget, forceClear);
    }

    OpenGLRenderer::OpenGLRenderer() {

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

    OpenGLRenderer::~OpenGLRenderer() {
        glfwDestroyWindow(window);
    }

    GLFWwindow *OpenGLRenderer::getWindow() {
        return window;
    }

    int OpenGLRenderer::getWindowWidth() {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        return width;
    }

    int OpenGLRenderer::getWindowHeight() {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        return height;
    }

    int OpenGLRenderer::getFramebufferWidth() {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        return width;
    }

    int OpenGLRenderer::getFramebufferHeight() {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        return height;
    }

}
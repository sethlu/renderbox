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

    OpenGLRenderList *OpenGLRenderer::prepassRender(Scene *scene, Camera *camera) {
        OpenGLRenderList *renderList = new OpenGLRenderList;

        std::queue<Object *> frontier;
        frontier.push((Object *) scene);

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

    void OpenGLRenderer::render(OpenGLRenderList *renderList, Scene *scene, Camera *camera, OpenGLRenderTarget *renderTarget, bool forceClear) {

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
            program->useProgram();

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
                switch (material->getMaterialType()) {
                    case MESH_BASIC_MATERIAL:
                        glUniform3fv(program->getUniformLocation("vertexColor"),
                                     1,
                                     glm::value_ptr(((MeshBasicMaterial *) material)->color));
                        break;
                    case MESH_LAMBERT_MATERIAL:
                        glUniformMatrix4fv(program->getUniformLocation("worldMatrix"),
                                           1,
                                           GL_FALSE,
                                           glm::value_ptr(object->getWorldMatrix()));
                        glUniformMatrix4fv(program->getUniformLocation("worldNormalMatrix"),
                                           1,
                                           GL_FALSE,
                                           glm::value_ptr(glm::transpose(glm::inverse(object->getWorldMatrix()))));
                        glUniform3fv(program->getUniformLocation("sceneAmbientColor"),
                                     1,
                                     glm::value_ptr(scene->getAmbientColor()));
                        glUniform3fv(program->getUniformLocation("vertexColor"),
                                     1,
                                     glm::value_ptr(((MeshBasicMaterial *) material)->color));
                        break;
                    case GLSL_SHADER_MATERIAL:
                        glUniformMatrix4fv(program->getUniformLocation("worldMatrix"),
                                           1,
                                           GL_FALSE,
                                           glm::value_ptr(object->getWorldMatrix()));
                        glUniformMatrix4fv(program->getUniformLocation("worldNormalMatrix"),
                                           1,
                                           GL_FALSE,
                                           glm::value_ptr(glm::transpose(glm::inverse(object->getWorldMatrix()))));
                        glUniform3fv(program->getUniformLocation("sceneAmbientColor"),
                                     1,
                                     glm::value_ptr(scene->getAmbientColor()));
                        break;
                    default: break;
                }

                // Bind vertex array
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
               scene, camera, renderTarget, forceClear);
    }

    int OpenGLRenderer::getFramebufferWidth() {
        return 0;
    }

    int OpenGLRenderer::getFramebufferHeight() {
        return 0;
    }

}
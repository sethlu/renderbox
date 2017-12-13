#include "platform.h"
#if defined(__MACOSX__)
#include <OpenGL/OpenGL.h>
#elif defined(__IPHONEOS__)
#include <OpenGLES/ES3/gl.h>
#endif
#include <iostream>
#include <queue>
#include <glm/gtc/type_ptr.hpp>
#include "Light.h"
#include "Matrix.h"
#include "Vector.h"
#include "OpenGLRenderer.h"


namespace renderbox {

    void OpenGLRenderer::render(Scene *scene, Camera *camera, OpenGLRenderTarget *renderTarget, bool forceClear) {

        // Prepass scene

        OpenGLRenderList renderList;

        std::queue<Object *> frontier;
        frontier.push(static_cast<Object *>(scene));

        while (!frontier.empty()) {
            auto object = frontier.front();
            frontier.pop();

            // Skip invisible objects
            if (!object->visible) continue;

            // Do not add objects without geometry or material
            if (object->hasGeometry() && object->hasMaterial()) {
                renderList.addObject(object->getMaterial().get(), object);
            }

            if (object->isLight()) {
                renderList.addLight(static_cast<Light *>(object));
            }

            for (const auto &next : object->getChildren()) {
                frontier.push(next.get());
            }
        }

        // Observe state changes

        bool invalidatePrograms = false;

        properties.numPointLights = static_cast<unsigned>(renderList.pointLights.size());
        if (properties.numPointLights != properties.lastNumPointLights) {
            properties.lastNumPointLights = properties.numPointLights;
            invalidatePrograms = true;
        }

        // Prepare

        // Use frame buffer from render target
        glBindFramebuffer(GL_FRAMEBUFFER, renderTarget ? renderTarget->getFramebufferId() : framebufferId);

        if (renderTarget) {
            glViewport(0, 0, renderTarget->getFramebufferWidth(), renderTarget->getFramebufferHeight());
        } else {
            glViewport(0, 0, getFramebufferWidth(), getFramebufferHeight());
        }

        // Clear the scene
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render

        // View projection matrix
        glm::mat4x4 viewProjectionMatrix = camera->getViewProjectionMatrix();

        for (const auto &it : renderList.objects) {

            // Use program
            OpenGLProgram *program = properties.getProgram(it.first, invalidatePrograms);
            program->useProgram();

            for (Object *object : it.second) {

                bool blankObjectProperties;
                OpenGLObjectProperties *objectProperties =
                    properties.getObjectProperties(object, &blankObjectProperties);

                OpenGLVertexArray *vertexArray = objectProperties->getVertexArray(0);

                if (blankObjectProperties) {

                    objectProperties->getBuffer(0)->buffer(object->getGeometry()->getVertices());
                    objectProperties->getBuffer(1)->buffer(object->getGeometry()->getNormals());
                    objectProperties->getBuffer(2)->buffer(object->getGeometry()->getFaces());
                    vertexArray->setElementBuffer(objectProperties->getBuffer(2));

                    goto UpdateVertexArray;

                } else if (invalidatePrograms) {

                    UpdateVertexArray:

                    vertexArray->setAttributeBuffer(program, "rb_vertexPosition", objectProperties->getBuffer(0));
                    vertexArray->enableAttribute(program, "rb_vertexPosition");

                    vertexArray->setAttributeBuffer(program, "rb_vertexNormal", objectProperties->getBuffer(1));
                    vertexArray->enableAttribute(program, "rb_vertexNormal");

                }

                // World projection matrix
                glm::mat4x4 worldProjectionMatrix = viewProjectionMatrix * object->getWorldMatrix();

                if (program->materialColor) {
                    if (auto material = dynamic_cast<ColorMaterial *>(object->getMaterial().get()))
                        glUniform3fv(program->getUniformLocation("rb_materialColor"),
                                     1,
                                     glm::value_ptr(material->getColor()));
                }
                if (program->pointLights && properties.numPointLights) {
                    glUniform3fv(program->getUniformLocation("rb_pointLights[0].position"),
                                 1,
                                 glm::value_ptr(renderList.pointLights[0]->getWorldPosition()));
                    glUniform3fv(program->getUniformLocation("rb_pointLights[0].color"),
                                 1,
                                 glm::value_ptr(renderList.pointLights[0]->getColor()));
                }
                if (program->worldMatrix) {
                    glUniformMatrix4fv(program->getUniformLocation("rb_worldMatrix"),
                                       1,
                                       GL_FALSE,
                                       glm::value_ptr(object->getWorldMatrix()));
                }
                if (program->sceneAmbientColor) {
                    glUniform3fv(program->getUniformLocation("rb_sceneAmbientColor"),
                                 1,
                                 glm::value_ptr(scene->getAmbientColor()));
                }
                if (program->worldNormalMatrix) {
                    glUniformMatrix4fv(program->getUniformLocation("rb_worldNormalMatrix"),
                                       1,
                                       GL_FALSE,
                                       glm::value_ptr(glm::transpose(glm::inverse(object->getWorldMatrix()))));
                }
                if (program->numActivePointLights) {
                    glUniform1i(program->getUniformLocation("rb_numActivePointLights"),
                                properties.numPointLights);
                }
                if (program->worldProjectionMatrix) {
                    glUniformMatrix4fv(program->getUniformLocation("rb_worldProjectionMatrix"),
                                       1,
                                       GL_FALSE,
                                       glm::value_ptr(worldProjectionMatrix));
                }

                // Bind vertex array
                vertexArray->bindVertexArray();

                // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                glDrawElements(GL_TRIANGLES, (GLsizei) object->getGeometry()->getFaces().size() * 3, GL_UNSIGNED_INT, 0);

            }
        }

        // Unbind vertex array
        OpenGLVertexArray::unbindVertexArray();

	}

    void OpenGLRenderer::loadObject(Object *object) {
        OpenGLObjectProperties *objectProperties = properties.getObjectProperties(object);
        OpenGLVertexArray *vertexArray = objectProperties->getVertexArray(0);

        objectProperties->getBuffer(0)->buffer(object->getGeometry()->getVertices());
        objectProperties->getBuffer(1)->buffer(object->getGeometry()->getNormals());
        objectProperties->getBuffer(2)->buffer(object->getGeometry()->getFaces());
        vertexArray->setElementBuffer(objectProperties->getBuffer(2));
    }

}

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

        // Enable depth tests
        glEnable(GL_DEPTH_TEST);

        // Clear the scene
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render

        // View projection matrix
        glm::mat4x4 viewProjectionMatrix = camera->getViewProjectionMatrix();

        for (const auto &it : renderList.objects) {

            // Use program
            OpenGLProgram *program = properties.getProgram(it.first, invalidatePrograms);
            program->useProgram();

            if (!program->pointLights.empty() && properties.numPointLights) {
                for (unsigned i = 0; i < properties.numPointLights; ++i) {
                    glUniform3fv(program->pointLights[i].position,
                                 1,
                                 glm::value_ptr(renderList.pointLights[i]->getWorldPosition()));
                    glUniform3fv(program->pointLights[i].color,
                                 1,
                                 glm::value_ptr(renderList.pointLights[i]->getColor()));
                }
            }
            if (program->sceneAmbientColor != -1) {
                glUniform3fv(program->sceneAmbientColor,
                             1,
                             glm::value_ptr(scene->getAmbientColor()));
            }
            if (program->numActivePointLights != -1) {
                glUniform1i(program->numActivePointLights,
                            properties.numPointLights);
            }

            for (Object *object : it.second) {

                bool blankObjectProperties;
                OpenGLObjectProperties *objectProperties =
                    properties.getObjectProperties(object, &blankObjectProperties);

                OpenGLVertexArray *vertexArray = objectProperties->getVertexArray(0);

                if (blankObjectProperties) {

                    objectProperties->getBuffer(0)->buffer(object->getGeometry()->getVertices());
                    objectProperties->getBuffer(1)->buffer(object->getGeometry()->getUVs());
                    objectProperties->getBuffer(2)->buffer(object->getGeometry()->getNormals());

                    objectProperties->getBuffer(3)->buffer(object->getGeometry()->getFaces());
                    vertexArray->setElementBuffer(objectProperties->getBuffer(3));

                    goto UpdateVertexArray;

                } else if (invalidatePrograms) {

                    UpdateVertexArray:

                    vertexArray->setAttributeBuffer(program, "rb_vertexPosition", objectProperties->getBuffer(0));
                    vertexArray->enableAttribute(program, "rb_vertexPosition");

                    vertexArray->setAttributeBuffer(program, "rb_vertexUV", objectProperties->getBuffer(1), 2);
                    vertexArray->enableAttribute(program, "rb_vertexUV");

                    vertexArray->setAttributeBuffer(program, "rb_vertexNormal", objectProperties->getBuffer(2));
                    vertexArray->enableAttribute(program, "rb_vertexNormal");

                }

                // World projection matrix
                glm::mat4x4 worldProjectionMatrix = viewProjectionMatrix * object->getWorldMatrix();

                if (program->materialAmbientColor != -1) {
                    if (auto material = dynamic_cast<AmbientMaterial *>(object->getMaterial().get()))
                        glUniform3fv(program->materialAmbientColor,
                                     1,
                                     glm::value_ptr(material->getAmbientColor()));
                }
                if (program->materialDiffuseColor != -1) {
                    if (auto material = dynamic_cast<DiffuseMaterial *>(object->getMaterial().get()))
                        glUniform3fv(program->materialDiffuseColor,
                                     1,
                                     glm::value_ptr(material->getDiffuseColor()));
                }
                if (program->worldMatrix != -1) {
                    glUniformMatrix4fv(program->worldMatrix,
                                       1,
                                       GL_FALSE,
                                       glm::value_ptr(object->getWorldMatrix()));
                }
                if (program->worldNormalMatrix != -1) {
                    glUniformMatrix4fv(program->worldNormalMatrix,
                                       1,
                                       GL_FALSE,
                                       glm::value_ptr(glm::transpose(glm::inverse(object->getWorldMatrix()))));
                }
                if (program->worldProjectionMatrix != -1) {
                    glUniformMatrix4fv(program->worldProjectionMatrix,
                                       1,
                                       GL_FALSE,
                                       glm::value_ptr(worldProjectionMatrix));
                }

                OpenGLTexture *diffuseMap(nullptr);
                if (object->getMaterial()->isDiffuseMaterial()) {
                    bool blankTexture;
                    diffuseMap = objectProperties->getTexture(0, &blankTexture);
                    if (blankTexture) {
                        if (auto material = dynamic_cast<DiffuseMaterial *>(object->getMaterial().get())) {
                            auto texture = material->getDiffuseMap(); // Need to check if texture exists
                            if (texture) diffuseMap->texture(material->getDiffuseMap().get());
                        }
                    }
                }
                if (program->materialDiffuseMap != -1 && diffuseMap) {
                    glActiveTexture(GL_TEXTURE0);
                    diffuseMap->bindTexture();
                    glUniform1i(program->materialDiffuseMap, 0);
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

        objectProperties->getBuffer(0)->buffer(object->getGeometry()->getVertices());
        objectProperties->getBuffer(1)->buffer(object->getGeometry()->getUVs());
        objectProperties->getBuffer(2)->buffer(object->getGeometry()->getNormals());

        objectProperties->getBuffer(3)->buffer(object->getGeometry()->getFaces());
    }

}

#include "OpenGLRenderer.h"

#include <iostream>
#include <queue>

#include <glm/gtc/type_ptr.hpp>

#include "Light.h"
#include "MeshGeometry.h"


namespace renderbox {

    void OpenGLRenderer::render(Scene *scene, Camera *camera, OpenGLRenderTarget *renderTarget) {

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
            if (object->hasGeometry() && object->hasMaterial() &&
                object->getMaterial()->supportsGeometry(object->getGeometry())) {
                renderList.addObject(object->getMaterial().get(), object);
            }

            if (object->isLight()) {
                renderList.addLight(static_cast<Light *>(object));
            }

            for (const auto &next : object->children) {
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
        glBindFramebuffer(GL_FRAMEBUFFER, renderTarget->getFramebufferId());

        glViewport(0, 0, renderTarget->getFramebufferWidth(), renderTarget->getFramebufferHeight());

        // Enable depth tests
        glEnable(GL_DEPTH_TEST);

        // Clear the scene
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render

        // View projection matrix
        mat4 viewProjectionMatrix = camera->getViewProjectionMatrix();

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

                auto const &material = object->getMaterial();

                // World projection matrix
                mat4 worldProjectionMatrix = viewProjectionMatrix * object->getWorldMatrix();

                if (program->materialAmbientColor != -1) {
                    if (auto ambientMaterial = dynamic_cast<AmbientMaterial *>(material.get()))
                        glUniform3fv(program->materialAmbientColor,
                                     1,
                                     glm::value_ptr(ambientMaterial->getAmbientColor()));
                }
                if (program->materialDiffuseColor != -1) {
                    if (auto diffuseMaterial = dynamic_cast<DiffuseMaterial *>(material.get()))
                        glUniform3fv(program->materialDiffuseColor,
                                     1,
                                     glm::value_ptr(diffuseMaterial->getDiffuseColor()));
                }
                if (program->worldMatrix != -1) {
                    glUniformMatrix4fv(program->worldMatrix,
                                       1,
                                       GL_FALSE,
                                       glm::value_ptr(object->getWorldMatrix()));
                }
                if (program->worldNormalMatrix != -1) {
                    glUniformMatrix3fv(program->worldNormalMatrix,
                                       1,
                                       GL_FALSE,
                                       glm::value_ptr(glm::transpose(glm::inverse(mat3(object->getWorldMatrix())))));
                }
                if (program->worldProjectionMatrix != -1) {
                    glUniformMatrix4fv(program->worldProjectionMatrix,
                                       1,
                                       GL_FALSE,
                                       glm::value_ptr(worldProjectionMatrix));
                }

                OpenGLTexture *ambientMap(nullptr);
                if (material->isAmbientMaterial()) {
                    bool blankTexture;
                    ambientMap = objectProperties->getTexture(0, &blankTexture);
                    if (blankTexture) {
                        if (auto ambientMaterial = dynamic_cast<AmbientMaterial *>(material.get())) {
                            auto texture = ambientMaterial->getAmbientMap(); // Need to check if texture exists
                            if (texture) ambientMap->texture(texture);
                        }
                    }
                }
                if (program->materialAmbientMap != -1 && ambientMap) {
                    glActiveTexture(GL_TEXTURE0);
                    ambientMap->bindTexture();
                    glUniform1i(program->materialAmbientMap, 0);
                }

                OpenGLTexture *diffuseMap(nullptr);
                if (material->isDiffuseMaterial()) {
                    bool blankTexture;
                    diffuseMap = objectProperties->getTexture(1, &blankTexture);
                    if (blankTexture) {
                        if (auto diffuseMaterial = dynamic_cast<DiffuseMaterial *>(material.get())) {
                            auto texture = diffuseMaterial->getDiffuseMap(); // Need to check if texture exists
                            if (texture) diffuseMap->texture(texture);
                        }
                    }
                }
                if (program->materialDiffuseMap != -1 && diffuseMap) {
                    glActiveTexture(GL_TEXTURE1);
                    diffuseMap->bindTexture();
                    glUniform1i(program->materialDiffuseMap, 1);
                }

                if (auto geometry = dynamic_cast<MeshGeometry *>(object->getGeometry().get())) {

                    OpenGLVertexArray *vertexArray = objectProperties->getVertexArray(0);

                    if (blankObjectProperties || objectProperties->geometryVersion != geometry->getVersion()) {
                        objectProperties->geometryVersion = geometry->getVersion();

                        objectProperties->getBuffer(0)->buffer(geometry->vertices);

                        if (geometry->uvs.size() == geometry->vertices.size())
                            objectProperties->getBuffer(1)->buffer(geometry->uvs);
                        else if (!geometry->uvs.empty()) throw 2;

                        if (geometry->normals.size() == geometry->vertices.size())
                            objectProperties->getBuffer(2)->buffer(geometry->normals);
                        else if (!geometry->normals.empty()) throw 2;

                        objectProperties->getBuffer(3)->buffer(geometry->faces);

                        vertexArray->setElementBuffer(objectProperties->getBuffer(3));

                        goto UpdateVertexArray;

                    } else if (invalidatePrograms) {

                        UpdateVertexArray:

                        auto buffer0(objectProperties->getBuffer(0));
                        if (buffer0->size) {
                            vertexArray->setAttributeBuffer(program, "rb_vertexPosition", buffer0);
                            vertexArray->enableAttribute(program, "rb_vertexPosition");
                        } else {
                            vertexArray->disableAttribute(program, "rb_vertexPosition");
                        }

                        auto buffer1(objectProperties->getBuffer(1));
                        if (buffer1->size) {
                            vertexArray->setAttributeBuffer(program, "rb_vertexUV", buffer1, 2);
                            vertexArray->enableAttribute(program, "rb_vertexUV");
                        } else {
                            vertexArray->disableAttribute(program, "rb_vertexUV");
                        }

                        auto buffer2(objectProperties->getBuffer(2));
                        if (buffer2->size) {
                            vertexArray->setAttributeBuffer(program, "rb_vertexNormal", buffer2);
                            vertexArray->enableAttribute(program, "rb_vertexNormal");
                        } else {
                            vertexArray->disableAttribute(program, "rb_vertexNormal");
                        }

                    }

                    // Bind vertex array
                    vertexArray->bindVertexArray();

                    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    glDrawElements(GL_TRIANGLES, (GLsizei) geometry->faces.size() * 3, GL_UNSIGNED_INT, 0);

                } else {
                    NOTREACHED() << "Only mesh geometry is supported" << std::endl;
                    exit(EXIT_FAILURE);
                }

            }
        }

        // Unbind vertex array
        OpenGLVertexArray::unbindVertexArray();

        renderTarget->frameDidRender();

	}

}

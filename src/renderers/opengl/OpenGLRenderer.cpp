#include "platform.h"
#if defined(__MACOSX__)
#include <OpenGL/OpenGL.h>
#elif defined(__IPHONEOS__)
#include <OpenGLES/ES3/gl.h>
#endif
#include <iostream>
#include <queue>
#include <glm/gtc/type_ptr.hpp>
#include "MeshLambertMaterial.h"
#include "MeshBasicMaterial.h"
#include "Matrix.h"
#include "OpenGLRenderer.h"


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

        objectProperties->getBuffer(0)->buffer(vertices);
        vertexArray->setAttributeBuffer(program, "vertexPosition", objectProperties->getBuffer(0));
        vertexArray->enableAttribute(program, "vertexPosition");

        objectProperties->getBuffer(1)->buffer(normals);
        vertexArray->setAttributeBuffer(program, "vertexNormal", objectProperties->getBuffer(1));
        vertexArray->enableAttribute(program, "vertexNormal");

        objectProperties->getBuffer(2)->buffer(faces);
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
                renderList->addObject(program->getProgramId(), current);
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
        glBindFramebuffer(GL_FRAMEBUFFER, renderTarget ? renderTarget->getFramebufferId() : framebufferId);

        if (renderTarget) {
            glViewport(0, 0, renderTarget->getFramebufferWidth(), renderTarget->getFramebufferHeight());
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
                                     glm::value_ptr(((MeshBasicMaterial *) material)->getColor()));
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
                                     glm::value_ptr(((MeshLambertMaterial *) material)->getColor()));
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
                glDrawElements(GL_TRIANGLES, (GLsizei) object->getGeometry()->getFaces().size() * 3, GL_UNSIGNED_INT, 0);

            }
        }

        // Unbind vertex array
        OpenGLVertexArray::unbindVertexArray();

    }

    void OpenGLRenderer::render(Scene *scene, Camera *camera, OpenGLRenderTarget *renderTarget, bool forceClear) {
        render(prepassRender(scene, camera), scene, camera, renderTarget, forceClear);
	}

}

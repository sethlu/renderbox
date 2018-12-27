#include "MetalRenderer.h"

#include <iostream>
#include <queue>

#import <simd/simd.h>

#include <glm/gtc/type_ptr.hpp>

#include "Light.h"
#include "Mesh.h"
#include "MetalRenderList.h"
#include "logging.h"


uint const NUM_MAX_POINT_LIGHTS = 4;
uint const NUM_MAX_BONES = 8;

typedef struct {
    simd_float3 position;
    simd_float3 color;
} PointLight;

typedef struct {
    matrix_float4x4 worldProjectionMatrix;
    matrix_float4x4 worldMatrix;
    matrix_float3x3 worldNormalMatrix;
    simd_float3 sceneAmbientColor;
    simd_float3 materialAmbientColor;
    simd_float3 materialDiffuseColor;
    uint numActivePointLights;
    PointLight pointLights[NUM_MAX_POINT_LIGHTS];
    matrix_float4x4 boneMatrics[NUM_MAX_BONES];
} Uniforms;

namespace renderbox {

    void MetalRenderer::render(Scene *scene, Camera *camera, SDLMetalRenderTarget *renderTarget) {

        @autoreleasepool {

            MetalView *metalView = renderTarget->metalView;
            id <MTLDevice> device = renderTarget->metalDevice;
            id <MTLCommandQueue> queue = renderTarget->metalQueue;

            MetalDeviceRendererProperties *deviceRendererProperties = properties.getDeviceRendererProperties(device);

            // Prepass scene

            MetalRenderList renderList;

            std::queue<Object *> frontier;
            frontier.push(static_cast<Object *>(scene));

            while (!frontier.empty()) {
                auto object = frontier.front();
                frontier.pop();

                // Skip invisible objects
                if (!object->visible) continue;

                // Do not add objects without geometry or material
                if (object->hasGeometry() && object->hasMaterial()) {
                    renderList.addObject(deviceRendererProperties->getRenderPipelineState(object->getMaterial().get()),
                                         object);
                }

                if (object->isLight()) {
                    renderList.addLight(static_cast<Light *>(object));
                }

                for (const auto &next : object->children) {
                    frontier.push(next.get());
                }
            }

            // Render

            id <CAMetalDrawable> drawable = [metalView.metalLayer nextDrawable];

            // Create command buffer
            id <MTLCommandBuffer> commandBuffer = [queue commandBuffer];

            // Create render pass descriptor
            MTLRenderPassDescriptor *renderPassDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];

            // Color attachment
            MTLRenderPassColorAttachmentDescriptor *renderPassColorAttachment = renderPassDescriptor.colorAttachments[0];
            renderPassColorAttachment.texture = drawable.texture;
            renderPassColorAttachment.loadAction = MTLLoadActionClear;
            renderPassColorAttachment.clearColor = MTLClearColorMake(0, 0, 0, 1);
            renderPassColorAttachment.storeAction = MTLStoreActionStore;

            // Depth attachment texture
            MTLTextureDescriptor *depthAttachmentTextureDescriptor =
                    [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatDepth32Float
                                                                       width:drawable.texture.width
                                                                      height:drawable.texture.height
                                                                   mipmapped:NO];
            depthAttachmentTextureDescriptor.resourceOptions = MTLResourceStorageModePrivate;
            depthAttachmentTextureDescriptor.usage = MTLTextureUsageRenderTarget;

            // Depth attachment
            MTLRenderPassDepthAttachmentDescriptor *renderPassDepthAttachment = renderPassDescriptor.depthAttachment;
            renderPassDepthAttachment.texture = [device newTextureWithDescriptor:depthAttachmentTextureDescriptor];
            renderPassDepthAttachment.loadAction = MTLLoadActionClear;
            renderPassDepthAttachment.clearDepth = 1.0f;
            renderPassDepthAttachment.storeAction = MTLStoreActionDontCare;

            // Depth stencil

            MTLDepthStencilDescriptor *depthStencilDescriptor = [MTLDepthStencilDescriptor new];
            depthStencilDescriptor.depthCompareFunction = MTLCompareFunctionLess;
            depthStencilDescriptor.depthWriteEnabled = YES;
            id <MTLDepthStencilState> depthStencilState = [device newDepthStencilStateWithDescriptor:depthStencilDescriptor];

            // Create render command encoder
            id <MTLRenderCommandEncoder> encoder =
                    [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
            [encoder setDepthStencilState:depthStencilState];
            [encoder setFrontFacingWinding:MTLWindingCounterClockwise];
            [encoder setCullMode:MTLCullModeNone];

            // View projection matrix
            mat4 viewProjectionMatrix =
                    mat4(1, 0, 0, 0,
                         0, 1, 0, 0,
                         0, 0, 0.5, 0,
                         0, 0, 0.5, 1) *
                    camera->getViewProjectionMatrix();

            // Uniforms
            Uniforms uniforms;

            // Scene ambient color
            memcpy(&uniforms.sceneAmbientColor,
                   glm::value_ptr(scene->getAmbientColor()),
                   sizeof(uniforms.sceneAmbientColor));

            // Point lights
            uniforms.numActivePointLights = renderList.pointLights.size();
            for (unsigned i = 0; i < uniforms.numActivePointLights; ++i) {
                memcpy(&uniforms.pointLights[i].position,
                       glm::value_ptr(renderList.pointLights[i]->getWorldPosition()),
                       sizeof(uniforms.pointLights[i].position));
                memcpy(&uniforms.pointLights[i].color,
                       glm::value_ptr(renderList.pointLights[i]->getColor()),
                       sizeof(uniforms.pointLights[i].color));
            }

            for (auto const &it : renderList.objects) {

                // Use the render pipeline state for the material
                [encoder setRenderPipelineState:it.first->renderPipelineStateObject.get()];

                for (Object *object : it.second) {

                    bool blankObjectProperties;
                    auto objectProperties =
                            deviceRendererProperties->getObjectProperties(object, &blankObjectProperties);

                    if (blankObjectProperties) {
                        objectProperties->uniformBuffer =
                            [device newBufferWithLength:sizeof(Uniforms) options:MTLResourceOptionCPUCacheModeDefault];
                    }

                    auto geometry = object->getGeometry();
                    auto material = object->getMaterial();

                    if (blankObjectProperties || objectProperties->geometryVersion != geometry->getVersion()) {
                        objectProperties->geometryVersion = geometry->getVersion();

                        auto numVertices = geometry->vertices.size();

                        objectProperties->getBuffer(0)->buffer(geometry->vertices);

                        if (geometry->uvs.size() == numVertices)
                            objectProperties->getBuffer(1)->buffer(geometry->uvs);
                        else if (!geometry->uvs.empty()) throw;

                        if (geometry->normals.size() == numVertices)
                            objectProperties->getBuffer(2)->buffer(geometry->normals);
                        else if (!geometry->normals.empty()) throw;

                        if (geometry->skinIndices.size() == numVertices)
                            objectProperties->getBuffer(3)->buffer(geometry->skinIndices);
                        else if (!geometry->skinIndices.empty()) throw;

                        if (geometry->skinWeights.size() == numVertices)
                            objectProperties->getBuffer(4)->buffer(geometry->skinWeights);
                        else if (!geometry->skinWeights.empty()) throw;

                        objectProperties->getBuffer(5)->buffer(geometry->faces);

                    }

                    // World projection matrix
                    mat4 worldProjectionMatrix = viewProjectionMatrix * object->getWorldMatrix();
                    memcpy(&uniforms.worldProjectionMatrix,
                           glm::value_ptr(worldProjectionMatrix),
                           sizeof(uniforms.worldProjectionMatrix));

                    // World matrix
                    memcpy(&uniforms.worldMatrix,
                           glm::value_ptr(object->getWorldMatrix()),
                           sizeof(uniforms.worldMatrix));

                    // World normal matrix
                    // Copy from mat4 since the memory alignment of mat3 is packed
                    memcpy(&uniforms.worldNormalMatrix,
                           glm::value_ptr(mat4(glm::transpose(glm::inverse(mat3(object->getWorldMatrix()))))),
                           sizeof(uniforms.worldNormalMatrix));

                    // Ambient material
                    if (material->isAmbientMaterial()) {
                        if (auto ambientMaterial = dynamic_cast<AmbientMaterial *>(material.get())) {
                            // Material ambient color

                            memcpy(&uniforms.materialAmbientColor,
                                   glm::value_ptr(ambientMaterial->getAmbientColor()),
                                   sizeof(uniforms.materialAmbientColor));

                            // Material ambient map

                            auto ambientMap = ambientMaterial->getAmbientMap();
                            if (ambientMap) {
                                bool blankTexture;
                                auto texture = objectProperties->getTexture(0, &blankTexture);
                                if (blankTexture) {
                                    texture->texture(ambientMap);
                                }
                                [encoder setFragmentTexture:texture->textureObject atIndex:0];
                            }
                        }
                    }

                    // Diffuse material
                    if (material->isDiffuseMaterial()) {
                        if (auto diffuseMaterial = dynamic_cast<DiffuseMaterial *>(material.get())) {
                            // Material diffuse color

                            memcpy(&uniforms.materialDiffuseColor,
                                   glm::value_ptr(diffuseMaterial->getDiffuseColor()),
                                   sizeof(uniforms.materialDiffuseColor));

                            // Material diffuse map

                            auto diffuseMap = diffuseMaterial->getDiffuseMap();
                            if (diffuseMap) {
                                bool blankTexture;
                                auto texture = objectProperties->getTexture(1, &blankTexture);
                                if (blankTexture) {
                                    texture->texture(diffuseMap);
                                }
                                [encoder setFragmentTexture:texture->textureObject atIndex:1];
                            }
                        }
                    }

                    // Bones
                    if (object->isMesh()) {
                        if (auto mesh = dynamic_cast<Mesh *>(object)) {
                            auto numBones = mesh->bones.size();
                            for (auto i = 0; i < numBones; i++) {
                                auto boneMatrix = glm::inverse(object->getWorldMatrix()) * mesh->bones[i]->getWorldMatrix() * mesh->bones[i]->getBoneInverse();
                                memcpy(&uniforms.boneMatrics[i], glm::value_ptr(boneMatrix), sizeof(uniforms.boneMatrics[0]));
                            }
                        } else {
                            NOTREACHED();
                        }
                    }

                    void *bufferPointer = [objectProperties->uniformBuffer contents];
                    memcpy(bufferPointer, &uniforms, sizeof(Uniforms));

                    [encoder setVertexBuffer:objectProperties->getBuffer(0)->bufferObject
                                      offset:0 atIndex:0];
                    [encoder setVertexBuffer:objectProperties->getBuffer(1)->bufferObject
                                      offset:0 atIndex:1];
                    [encoder setVertexBuffer:objectProperties->getBuffer(2)->bufferObject
                                      offset:0 atIndex:2];
                    [encoder setVertexBuffer:objectProperties->uniformBuffer
                                      offset:0 atIndex:3];
                    [encoder setVertexBuffer:objectProperties->getBuffer(3)->bufferObject
                                      offset:0 atIndex:4];
                    [encoder setVertexBuffer:objectProperties->getBuffer(4)->bufferObject
                                      offset:0 atIndex:5];

                    [encoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle
                                        indexCount:object->getGeometry()->faces.size() * 3
                                         indexType:MTLIndexTypeUInt32
                                       indexBuffer:objectProperties->getBuffer(5)->bufferObject
                                 indexBufferOffset:0];

                }

            }

            // End encoding
            [encoder endEncoding];

            // Commit command buffer
            [commandBuffer presentDrawable:drawable];
            [commandBuffer commit];

        }

    }

}

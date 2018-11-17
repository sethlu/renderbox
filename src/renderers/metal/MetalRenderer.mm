#include "MetalRenderer.h"

#include <iostream>
#include <queue>

#import <simd/simd.h>

#include <glm/gtc/type_ptr.hpp>

#include "Light.h"
#include "MetalRenderList.h"


#define NUM_POINT_LIGHTS 4

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
    PointLight pointLights[NUM_POINT_LIGHTS];
} Uniforms;

namespace renderbox {

    void MetalRenderer::render(Scene *scene, Camera *camera, SDLMetalRenderTarget *renderTarget) {

        @autoreleasepool {

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
                    renderList.addObject(object->getMaterial().get(), object);
                }

                if (object->isLight()) {
                    renderList.addLight(static_cast<Light *>(object));
                }

                for (const auto &next : object->children) {
                    frontier.push(next.get());
                }
            }

            // Render

            MetalView *metalView = renderTarget->metalView;
            id <MTLDevice> device = metalView.metalLayer.device;
            id <MTLCommandQueue> queue = renderTarget->queue;
            id <CAMetalDrawable> drawable = [metalView.metalLayer nextDrawable];

            MetalDeviceRendererProperties *deviceRendererProperties = properties.getDeviceRendererProperties(device);

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
            id <MTLBuffer> uniformBuffer = [device newBufferWithLength:sizeof(Uniforms) options:MTLResourceOptionCPUCacheModeDefault];
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

            for (const auto &it : renderList.objects) {

                // Use the render pipeline state for the material
                auto renderPipelineState = deviceRendererProperties->getRenderPipelineState(it.first);
                [encoder setRenderPipelineState:renderPipelineState->renderPipelineStateObject.get()];

                for (Object *object : it.second) {

                    bool blankObjectProperties;
                    auto objectProperties =
                            deviceRendererProperties->getObjectProperties(object, &blankObjectProperties);

                    if (blankObjectProperties) {

                        auto geometry = object->getGeometry();

                        objectProperties->getBuffer(0)->buffer(geometry->vertices);

                        if (geometry->uvs.size() == geometry->vertices.size())
                            objectProperties->getBuffer(1)->buffer(geometry->uvs);
                        else if (!geometry->uvs.empty()) throw;

                        if (geometry->normals.size() == geometry->vertices.size())
                            objectProperties->getBuffer(2)->buffer(geometry->normals);
                        else if (!geometry->normals.empty()) throw;

                        objectProperties->getBuffer(3)->buffer(geometry->faces);

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

                    // Material ambient color
                    if (object->getMaterial()->isAmbientMaterial()) {
                        if (auto material = dynamic_cast<AmbientMaterial *>(object->getMaterial().get())) {
                            memcpy(&uniforms.materialAmbientColor,
                                   glm::value_ptr(material->getAmbientColor()),
                                   sizeof(uniforms.materialAmbientColor));
                        }
                    }

                    // Material diffuse color
                    if (object->getMaterial()->isDiffuseMaterial()) {
                        if (auto material = dynamic_cast<DiffuseMaterial *>(object->getMaterial().get())) {
                            memcpy(&uniforms.materialDiffuseColor,
                                   glm::value_ptr(material->getDiffuseColor()),
                                   sizeof(uniforms.materialDiffuseColor));
                        }
                    }

                    void *bufferPointer = [uniformBuffer contents];
                    memcpy(bufferPointer, &uniforms, sizeof(Uniforms));

                    [encoder setVertexBuffer:objectProperties->getBuffer(0)->bufferObject
                                      offset:0 atIndex:0];
                    [encoder setVertexBuffer:objectProperties->getBuffer(1)->bufferObject
                                      offset:0 atIndex:1];
                    [encoder setVertexBuffer:objectProperties->getBuffer(2)->bufferObject
                                      offset:0 atIndex:2];
                    [encoder setVertexBuffer:uniformBuffer
                                      offset:0 atIndex:3];

                    [encoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle
                                        indexCount:object->getGeometry()->getFaces().size() * 3
                                         indexType:MTLIndexTypeUInt32
                                       indexBuffer:objectProperties->getBuffer(3)->bufferObject
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

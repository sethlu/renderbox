#include "MetalRenderer.h"

#include <iostream>
#include <queue>

#import <simd/simd.h>

#include <glm/gtc/type_ptr.hpp>

#include "Light.h"
#include "MetalRenderList.h"


typedef struct {
    matrix_float4x4 worldProjectionMatrix;
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

            // Set up to render

            NSError *err = nil;

            MetalView *metalView = renderTarget->metalView;
            id<MTLDevice> device = metalView.metalLayer.device;
            id<MTLCommandQueue> queue = renderTarget->queue;

            MetalDeviceRendererProperties *deviceRendererProperties = properties.getDeviceRendererProperties(device);

            id<MTLRenderPipelineState> renderPipelineState =
                    [device newRenderPipelineStateWithDescriptor:deviceRendererProperties->renderPipelineDescriptor error:&err];
            assert(err == nil);

            id<MTLBuffer> uniformBuffer = [device newBufferWithLength:sizeof(Uniforms) options:MTLResourceOptionCPUCacheModeDefault];

            // Render

            id<CAMetalDrawable> drawable = [metalView.metalLayer nextDrawable];

            // Create render pass descriptor
            MTLRenderPassDescriptor *renderPassDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];
            MTLRenderPassColorAttachmentDescriptor *renderPassColorAttachment = renderPassDescriptor.colorAttachments[0];
            renderPassColorAttachment.texture = drawable.texture;
            renderPassColorAttachment.loadAction = MTLLoadActionClear;
            renderPassColorAttachment.clearColor = MTLClearColorMake(0, 0, 0, 1);
            renderPassColorAttachment.storeAction = MTLStoreActionStore;

            // Create command buffer
            id<MTLCommandBuffer> commandBuffer = [queue commandBuffer];

            // Create render command encoder
            // Implicit clear color
            id<MTLRenderCommandEncoder> encoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];

            [encoder setRenderPipelineState:renderPipelineState];
            [encoder setFrontFacingWinding:MTLWindingCounterClockwise];
            [encoder setCullMode:MTLCullModeNone];

            // View projection matrix
            mat4 viewProjectionMatrix = camera->getViewProjectionMatrix();

            for (const auto &it : renderList.objects) {

                for (Object *object : it.second) {

                    bool blankObjectProperties;
                    auto objectProperties =
                            deviceRendererProperties->getObjectProperties(object, &blankObjectProperties);

                    if (blankObjectProperties) {

                        auto geometry = object->getGeometry();

                        objectProperties->getBuffer(0)->buffer(geometry->vertices);
                        objectProperties->getBuffer(1)->buffer(geometry->faces);

                    }

                    // World projection matrix
                    mat4 worldProjectionMatrix = viewProjectionMatrix * object->getWorldMatrix();

                    Uniforms uniforms;
                    memcpy(&uniforms.worldProjectionMatrix,
                           glm::value_ptr(worldProjectionMatrix),
                           sizeof(uniforms.worldProjectionMatrix));
                    void *bufferPointer = [uniformBuffer contents];
                    memcpy(bufferPointer, &uniforms, sizeof(Uniforms));

                    [encoder setVertexBuffer:objectProperties->getBuffer(0)->bufferObject offset:0 atIndex:0];
                    [encoder setVertexBuffer:uniformBuffer offset:0 atIndex:1];

                    [encoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle
                                        indexCount:object->getGeometry()->getFaces().size() * 3
                                         indexType:MTLIndexTypeUInt32
                                       indexBuffer:objectProperties->getBuffer(1)->bufferObject
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

#ifndef RENDERBOX_METALSDLRENDERER_H
#define RENDERBOX_METALSDLRENDERER_H


#import <Cocoa/Cocoa.h>
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

#include <SDL.h>

#include "Scene.h"
#include "Camera.h"

@interface MetalView : NSView

@property(nonatomic) CAMetalLayer *metalLayer;

@end

namespace renderbox {

    class SDLMetalRenderTarget {

        friend class MetalRenderer;

    public:

        SDLMetalRenderTarget();

        virtual ~SDLMetalRenderTarget();

        SDL_Window *getWindow() const;

        virtual int getFramebufferWidth() const;

        virtual int getFramebufferHeight() const;

    protected:

        SDL_Window *window;

        SDL_GLContext context;

        MetalView *metalView;

        id<MTLCommandQueue> queue;

        MTLRenderPassDescriptor *renderPassDescriptor;

    };

}


#endif //RENDERBOX_METALSDLRENDERER_H

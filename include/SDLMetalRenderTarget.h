#ifndef RENDERBOX_METALSDLRENDERER_H
#define RENDERBOX_METALSDLRENDERER_H


#include "platform.h"
#ifdef RENDERBOX_OS_MACOS
#import <Cocoa/Cocoa.h>
#else
#import <UIKit/UIView.h>
#endif

#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

#include <SDL.h>
#include <SDL_syswm.h>

#include "Scene.h"
#include "Camera.h"
#include "scoped_nsobject.h"


#ifdef RENDERBOX_OS_MACOS

@interface MetalView : NSView

@property(nonatomic) CAMetalLayer *metalLayer;

@end

#else

@interface MetalView : UIView

@property(nonatomic) CAMetalLayer *metalLayer;

@end

#endif

namespace renderbox {

    class SDLMetalRenderTarget {

        friend class MetalRenderer;

    public:

        SDLMetalRenderTarget();

        virtual ~SDLMetalRenderTarget();

        SDL_Window *getWindow() const;

        int getWindowWidth() const;

        int getWindowHeight() const;

        virtual int getFramebufferWidth() const;

        virtual int getFramebufferHeight() const;

    protected:

        SDL_Window *sdlWindow;

        SDL_SysWMinfo info;

#ifdef RENDERBOX_OS_MACOS
        NSView *sdlView;
#else
        UIView *sdlView;
#endif

        SDL_GLContext context;

        MetalView *metalView;

        id <MTLDevice> metalDevice;

        scoped_nsprotocol<id <MTLCommandQueue>> metalQueue;

        MTLRenderPassDescriptor *renderPassDescriptor;

    };

}


#endif //RENDERBOX_METALSDLRENDERER_H

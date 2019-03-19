#include "SDLOpenGLRenderTarget.h"

#if defined(RENDERBOX_OS_MACOS)
#import <AppKit/AppKit.h>
#endif //defined(RENDERBOX_OS_MACOS)

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

namespace renderbox {

    SDLOpenGLRenderTarget::SDLOpenGLRenderTarget(Uint32 additionalInitFlags) {

        // SDL

        if (SDL_Init(SDL_INIT_VIDEO | additionalInitFlags) < 0) {
            fprintf(stderr, "Failed to initialize SDL\n");
            throw 2;
        }

#if defined(RENDERBOX_OS_MACOS)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#elif defined(RENDERBOX_OS_IPHONEOS)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#endif
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        // Create window
        
#if defined(RENDERBOX_OS_MACOS)
        window = SDL_CreateWindow(nullptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                WINDOW_WIDTH, WINDOW_HEIGHT,
                SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
#elif defined(RENDERBOX_OS_IPHONEOS)
        SDL_DisplayMode displayMode;
        SDL_GetDesktopDisplayMode(0, &displayMode);
        
        window = SDL_CreateWindow(nullptr, 0, 0,
                displayMode.w, displayMode.h,
                SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
#endif
        
        if (!window) {
            fprintf(stderr, "Failed to init window\n");
            throw 2;
        }

        context = SDL_GL_CreateContext(window);
        if (!context) {
            fprintf(stderr, "Failed to create context\n");
            throw 2;
        }

		SDL_GL_SetSwapInterval(1);
		
		// Update frame buffer id
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint *) &framebufferId);

    }

    SDLOpenGLRenderTarget::~SDLOpenGLRenderTarget() {
		
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(window);
		
    }

    SDL_Window *SDLOpenGLRenderTarget::getWindow() const {
        return window;
    }

    unsigned int SDLOpenGLRenderTarget::getFramebufferWidth() const {
		int width;
		SDL_GL_GetDrawableSize(window, &width, nullptr);
        return static_cast<unsigned int>(width);
    }

    unsigned int SDLOpenGLRenderTarget::getFramebufferHeight() const {
		int height;
		SDL_GL_GetDrawableSize(window, nullptr, &height);
        return static_cast<unsigned int>(height);
    }
    
    void SDLOpenGLRenderTarget::frameDidRender() {
        OpenGLRenderTarget::frameDidRender();
        
#if defined(RENDERBOX_OS_MACOS)
        // The following is a rough fix to OpenGL drawing resulting in empty screen on macOS Mojave
        // Ref: https://github.com/glfw/glfw/issues/1334
        if (frameCount <= 2) [(__bridge NSOpenGLContext *) context update];
#endif //defined(RENDERBOX_OS_MACOS)
    }

}

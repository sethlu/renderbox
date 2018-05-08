
#if defined(__MACOSX__)
#include <OpenGL/gl3.h>
#elif defined(__IPHONEOS__)
#include <OpenGLES/ES3/gl.h>
#endif
#include "OpenGLSDLRenderer.h"


#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 568

namespace renderbox {

    OpenGLSDLRenderer::OpenGLSDLRenderer(Uint32 additionalInitFlags) {

        // SDL

        if (SDL_Init(SDL_INIT_VIDEO | additionalInitFlags) < 0) {
            fprintf(stderr, "Failed to initialize SDL\n");
            throw 2;
        }

        #if defined(__MACOSX__)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        #elif defined(__IPHONEOS__)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
        #endif
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        // Create window
		
        window = SDL_CreateWindow(NULL, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
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

    OpenGLSDLRenderer::~OpenGLSDLRenderer() {
		
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(window);
		
    }

    SDL_Window *OpenGLSDLRenderer::getWindow() const {
        return window;
    }

    int OpenGLSDLRenderer::getFramebufferWidth() const {
		int width;
		SDL_GL_GetDrawableSize(window, &width, nullptr);
        return width;
    }

    int OpenGLSDLRenderer::getFramebufferHeight() const {
		int height;
		SDL_GL_GetDrawableSize(window, nullptr, &height);
        return height;
    }

}

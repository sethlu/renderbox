#ifndef RENDERBOX_OPENGLSDLRENDERER_H
#define RENDERBOX_OPENGLSDLRENDERER_H


#include <SDL.h>

#include "OpenGLRenderer.h"

namespace renderbox {

    class SDLOpenGLRenderTarget : public OpenGLRenderTarget {
    public:

        explicit SDLOpenGLRenderTarget(Uint32 additionalInitFlags = 0);

        ~SDLOpenGLRenderTarget() override;

        SDL_Window *getWindow() const;

        unsigned int getFramebufferWidth() const override;

        unsigned int getFramebufferHeight() const override;

        void frameDidRender() override;

    protected:

        SDL_Window *window;

        SDL_GLContext context;

    };

}

#endif //RENDERBOX_OPENGLSDLRENDERER_H

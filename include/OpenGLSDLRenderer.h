#ifndef VOX_OPENGLSDLRENDERER_H
#define VOX_OPENGLSDLRENDERER_H


#include "platform.h"
#if defined(__IPHONEOS__)

#include <SDL.h>
#include "OpenGLRenderer.h"

namespace renderbox {

    class OpenGLSDLRenderer : public OpenGLRenderer {
    protected:

        SDL_Window *window;

        SDL_GLContext context;

    public:

        OpenGLSDLRenderer();

        ~OpenGLSDLRenderer();

        SDL_Window *getWindow() const;

		int getFramebufferWidth() const override;

        int getFramebufferHeight() const override;

    };

}

#endif // defined(__IPHONEOS__)


#endif //VOX_OPENGLSDLRENDERER_H

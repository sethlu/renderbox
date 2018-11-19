#ifndef RENDERBOX_OPENGL_H
#define RENDERBOX_OPENGL_H


#include "platform.h"

#if defined(RENDERBOX_OS_MACOS)
#include <OpenGL/gl3.h>
#elif defined(RENDERBOX_OS_IPHONEOS)
#include <OpenGLES/ES3/gl.h>
#endif


#endif //RENDERBOX_OPENGL_H

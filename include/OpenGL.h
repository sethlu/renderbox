#ifndef RENDERBOX_OPENGL_H
#define RENDERBOX_OPENGL_H


#include "platform.h"

#if defined(RENDERBOX_OS_MACOS)

#define RENDERBOX_OPENGL_GLSL_VERSION 330

#include <OpenGL/gl3.h>

#elif defined(RENDERBOX_OS_IPHONEOS)

#define RENDERBOX_OPENGL_GLSL_VERSION 300ES

#include <OpenGLES/ES3/gl.h>

#elif defined(RENDERBOX_OS_LINUX)

#define RENDERBOX_OPENGL_GLSL_VERSION 120

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glcorearb.h>
#include <GL/glext.h>

#endif


#endif //RENDERBOX_OPENGL_H

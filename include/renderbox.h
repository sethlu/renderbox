#ifndef RENDERBOX_RENDERBOX_H
#define RENDERBOX_RENDERBOX_H


#include "Object.h"

#include "Vector.h"
#include "Matrix.h"
#include "Ray.h"

#include "Camera.h"
#include "PerspectiveCamera.h"

#include "Geometry.h"
#include "BoxGeometry.h"
#include "VoxelGeometry.h"

#include "Material.h"
#include "MeshBasicMaterial.h"
#include "MeshLambertMaterial.h"
#include "GLSLMaterial.h"

#include "Mesh.h"

#include "Light.h"
#include "PointLight.h"

#include "Scene.h"

#if defined(RENDERBOX_USE_OPENGL)
#include "OpenGLRenderer.h"
#endif

#if defined(RENDERBOX_USE_GLFW) && defined(RENDERBOX_USE_OPENGL)
#include "GLFWOpenGLRenderTarget.h"
#endif

#if defined(RENDERBOX_USE_SDL) && defined(RENDERBOX_USE_OPENGL)
#include "SDLOpenGLRenderTarget.h"
#endif

#include "GLSLPreprocessor.h"

#include "OBJLoader.h"


#endif //RENDERBOX_RENDERBOX_H

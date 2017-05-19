#include <GL/glew.h>
#include <map>
#include "../../scenes/Scene.h"
#include "../../cameras/Camera.h"
#include "../../objects/Mesh.h"
#include "../../../../libs/glfw/include/GLFW/glfw3.h"
#include "OpenGLRenderTarget.h"
#include "OpenGLRendererProperties.h"


#ifndef PARTICLESIM_RENDERER_H
#define PARTICLESIM_RENDERER_H


namespace renderbox {

    class Renderer {
        GLFWwindow *window;
        OpenGLRendererProperties *properties = new OpenGLRendererProperties();
        void renderObject(Object *object, Camera *camera);
        void renderObject(Object *object, Camera *camera, glm::mat4x4 viewProjectionMatrix);
    public:
        Renderer();
        ~Renderer();
        GLFWwindow *getWindow();
        void render(Scene *scene, Camera *camera, OpenGLRenderTarget *renderTarget = nullptr, bool forceClear = false);
        void loadObject(Object *object);
        int getWindowWidth();
        int getWindowHeight();
        int getFramebufferWidth();
        int getFramebufferHeight();
    };

}


#endif //PARTICLESIM_RENDERER_H

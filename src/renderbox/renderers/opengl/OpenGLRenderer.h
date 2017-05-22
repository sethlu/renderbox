#include <GL/glew.h>
#include <map>
#include "../../scenes/Scene.h"
#include "../../cameras/Camera.h"
#include "../../objects/Mesh.h"
#include "../../../../libs/glfw/include/GLFW/glfw3.h"
#include "OpenGLRenderTarget.h"
#include "OpenGLRendererProperties.h"
#include "OpenGLRenderList.h"


#ifndef PARTICLESIM_OPENGLRENDERER_H
#define PARTICLESIM_OPENGLRENDERER_H


namespace renderbox {

    class OpenGLRenderer {

        static std::unordered_map<int, OpenGLProgram *> programs;

        GLFWwindow *window;
        OpenGLRendererProperties *properties = new OpenGLRendererProperties();

    public:

        OpenGLRenderer();
        ~OpenGLRenderer();

        void loadObject(Object *object);

        OpenGLRenderList * prepassRender(Object *object, Camera *camera);
        void render(OpenGLRenderList *renderList, Camera *camera, OpenGLRenderTarget *renderTarget, bool forceClear);
        void render(Scene *scene, Camera *camera, OpenGLRenderTarget *renderTarget = nullptr, bool forceClear = false);

        GLFWwindow *getWindow();
        int getWindowWidth();
        int getWindowHeight();
        int getFramebufferWidth();

        int getFramebufferHeight();

    };

}


#endif //PARTICLESIM_OPENGLRENDERER_H

#include <iostream>
#include "renderbox.h"

std::unique_ptr<renderbox::OpenGLGLFWRenderer> renderer;
std::shared_ptr<renderbox::PerspectiveCamera> camera;
std::shared_ptr<renderbox::Object> suzanne;

void windowSizeCallback(GLFWwindow *window, int width, int height) {
    camera->setPerspective(glm::radians(45.0f), (float) renderer->getWindowWidth() / (float) renderer->getWindowHeight());
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    suzanne->rotate(glm::vec3(0, 1, 0), static_cast<float>(xoffset) * 0.05f);
    suzanne->rotate(glm::vec3(1, 0, 0), static_cast<float>(yoffset) * 0.05f);
}

int main(int argc, char **argv) {

    renderer.reset(new renderbox::OpenGLGLFWRenderer);
    GLFWwindow *window = renderer->getWindow();

    glfwSetWindowSizeCallback(window, windowSizeCallback);
    glfwSetScrollCallback(window, scrollCallback);

    auto scene = std::make_shared<renderbox::Scene>();
    scene->setAmbientColor(glm::vec3(0.05f));

    renderbox::OBJLoader loader(scene);
    loader.enterOBJSourceFile("suzanne.obj");
    suzanne = scene->children[0];

    for (unsigned i = 0; i < 8; ++i) {
        auto pointLight = std::make_shared<renderbox::PointLight>(60.0f * normalize(glm::vec3(
            (float) rand() / RAND_MAX, (float) rand() / RAND_MAX, (float) rand() / RAND_MAX)));
        pointLight->setTranslation(glm::vec3(-32 + 64 * (float) rand() / RAND_MAX,
                                             -32 + 64 * (float) rand() / RAND_MAX,
                                             -32 + 64 * (float) rand() / RAND_MAX));
        scene->addChild(pointLight);
    }

    camera = std::make_shared<renderbox::PerspectiveCamera>(
        glm::radians(45.0f), (float) renderer->getWindowWidth() / (float) renderer->getWindowHeight());
    camera->setTranslation(glm::vec3(0, 0, 5));

    double lastTime = glfwGetTime();
    int nFrames = 0;

    while (!glfwWindowShouldClose(window)) {

        double currentTime = glfwGetTime();
        ++nFrames;
        if (currentTime - lastTime >= 1.0){
            fprintf(stdout, "%.1f ms/frame\t%d fps\n", 1000.0 / double(nFrames), nFrames);
            nFrames = 0;
            lastTime = currentTime;
        }

        renderer->render(scene.get(), camera.get());

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwTerminate();

}
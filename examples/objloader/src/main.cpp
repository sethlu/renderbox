#include <iostream>
#include "renderbox.h"

int main(int argc, char **argv) {

    std::unique_ptr<renderbox::OpenGLGLFWRenderer> renderer(new renderbox::OpenGLGLFWRenderer);
    GLFWwindow *window = renderer->getWindow();
    auto scene = std::make_shared<renderbox::Scene>();
    scene->setAmbientColor(glm::vec3(0.05f));
    auto camera = std::make_shared<renderbox::PerspectiveCamera>(
        glm::radians(45.0f), (float) renderer->getWindowWidth() / (float) renderer->getWindowHeight());
    camera->setTranslation(glm::vec3(0, 0, 5));

    renderbox::OBJLoader loader(scene);
    loader.enterOBJSourceFile("suzanne.obj");

    for (unsigned i = 0; i < 5; ++i) {
        auto pointLight = std::make_shared<renderbox::PointLight>(30.0f * normalize(glm::vec3(
            (float) rand() / RAND_MAX, (float) rand() / RAND_MAX, (float) rand() / RAND_MAX)));
        pointLight->setTranslation(glm::vec3(-32 + 64 * (float) rand() / RAND_MAX,
                                             -32 + 64 * (float) rand() / RAND_MAX,
                                             -32 + 64 * (float) rand() / RAND_MAX));
        scene->addChild(pointLight);
    }

    while (!glfwWindowShouldClose(window)) {

        renderer->render(scene.get(), camera.get());

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwTerminate();

}
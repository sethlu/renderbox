#include <iostream>
#include <glm/glm.hpp>
#define RENDERBOX_USE_OPENGL
#define RENDERBOX_USE_GLFW
#include "renderbox.h"

std::shared_ptr<renderbox::Scene> scene;
std::shared_ptr<renderbox::PerspectiveCamera> camera;
std::unique_ptr<renderbox::OpenGLRenderer> renderer;
std::unique_ptr<renderbox::GLFWOpenGLRenderTarget> renderTarget;

std::shared_ptr<renderbox::Object> cameraRig;
std::shared_ptr<renderbox::Object> terrain;
std::shared_ptr<renderbox::Object> testCube;

float cameraDistance = 40.0f;
float cameraAngularVelocity = 0.0f;
float cameraAngle[] = {0.0f, 45.0f};
float cameraVelocity[] = {0.0f, 0.0f}; // Relative to the camera forward direction

float lastTime = (float) glfwGetTime();
double mouseX = -1, mouseY = -1, startMouseX = -1, startMouseY = -1;

float isolevel = 0.5f;

void init() {

    // Scene
    scene = std::make_shared<renderbox::Scene>();
    scene->setAmbientColor(glm::vec3(0.01f));

    // Voxel terrain
    auto voxelGeometry = std::make_shared<renderbox::VoxelGeometry>();
    for (int x = -32; x < 32; ++x) {
        for (int y = -32; y < 32; ++y) {
            voxelGeometry->setOccupancy(x, y, 0, 1);
        }
    }
    voxelGeometry->updateGeometryByMarchingCube(isolevel); // Refresh geometry
    terrain = std::make_shared<renderbox::Object>(
        voxelGeometry,
        std::make_shared<renderbox::MeshLambertMaterial>(glm::vec3(1.0f)));
    scene->addChild(terrain);

    // Test cube
    testCube = std::make_shared<renderbox::Object>(
        std::make_shared<renderbox::BoxGeometry>(0.5f, 0.5f, 0.5f),
        std::make_shared<renderbox::MeshBasicMaterial>(glm::vec3(1.0f, 0, 0)));
    scene->addChild(testCube);

    // Camera
    camera = std::make_shared<renderbox::PerspectiveCamera>(
        glm::radians(45.0f), (float) renderTarget->getWindowWidth() / (float) renderTarget->getWindowHeight());
    camera->setTranslation(glm::vec3(0, 0, cameraDistance));
    cameraRig = std::make_shared<renderbox::Object>();
    cameraRig->addChild(camera);
    cameraRig->rotate(glm::vec3(1.0f, 0, 0), glm::radians(cameraAngle[1]));

    // Light

    for (unsigned i = 0; i < 8; ++i) {
        auto pointLight = std::make_shared<renderbox::PointLight>(100.0f * normalize(glm::vec3(
            (float) rand() / RAND_MAX, (float) rand() / RAND_MAX, (float) rand() / RAND_MAX)));
        pointLight->setTranslation(glm::vec3(-32 + 64 * (float) rand() / RAND_MAX,
                                             -32 + 64 * (float) rand() / RAND_MAX,
                                                   32));
        scene->addChild(pointLight);
    }

}

double mouseLastSync = glfwGetTime();

void update() {

    float currentTime = (float) glfwGetTime();
    float deltaTime = currentTime - lastTime;

    // Geometry

    if (currentTime - mouseLastSync > 0.04f
        && startMouseX != -1 && startMouseY != -1) {

        auto cameraRay = camera->getRay(glm::vec2(
                2 * mouseX / renderTarget->getWindowWidth() - 1.0f,
                1.0f - 2 * mouseY / renderTarget->getWindowHeight()));
        std::vector<glm::vec3> worldPositions;
        if (cameraRay.intersectObject(terrain.get(), worldPositions)) {
            glm::vec3 objectPosition = floor(renderbox::dehomogenize(
                    glm::inverse(terrain->getWorldMatrix())
                    * glm::vec4(worldPositions[0] + glm::vec3(0.5f), 1.0f)));

            renderbox::VoxelGeometry *voxelGeometry = (renderbox::VoxelGeometry *) terrain->getGeometry().get();

            voxelGeometry->brush(objectPosition, 8, 0.4f, isolevel);

            voxelGeometry->updateGeometryByMarchingCube(isolevel);
        }

        mouseLastSync = currentTime;
    }

    // Test cube

    testCube->visible = false;
    if (mouseX >= 0 && mouseX <= renderTarget->getWindowWidth() && mouseY >= 0 && mouseY <= renderTarget->getWindowHeight()) {
        auto cameraRay = camera->getRay(glm::vec2(
                2 * mouseX / renderTarget->getWindowWidth() - 1.0f,
                1.0f - 2 * mouseY / renderTarget->getWindowHeight()));
        std::vector<glm::vec3> worldPositions;
        if (cameraRay.intersectObject(terrain.get(), worldPositions)) {
            glm::vec3 testLocation = worldPositions[0];
            testCube->visible = true;
            testCube->setTranslation(testLocation);
        }
    }

    // Camera

    auto cameraDirection = camera->getRay(renderbox::vec2()).getDirection();
    auto forward = glm::normalize(glm::vec3(cameraDirection.x, cameraDirection.y, 0));
    auto right = glm::normalize(glm::vec3(cameraDirection.y, - cameraDirection.x, 0));
    cameraRig->translate(glm::vec3(right * cameraVelocity[0] + forward * cameraVelocity[1])
                         * deltaTime * cameraDistance * 0.01f);
    cameraRig->clearRotation();
    cameraAngle[0] += cameraAngularVelocity * deltaTime;
    if (cameraAngle[1] < 10.0f) cameraAngle[1] = 10.0f;
    else if (cameraAngle[1] > 85.0f) cameraAngle[1] = 85.0f;
    cameraRig->rotate(glm::vec3(1.0f, 0, 0), glm::radians(cameraAngle[1]));
    cameraRig->rotate(glm::vec3(0, 0, 1.0f), glm::radians(cameraAngle[0]));

    // Render

    renderer->render(scene.get(), camera.get(), renderTarget.get());

    lastTime = currentTime;

}

void uninit() {

}

void mousedown(GLFWwindow *window) {

}

void mousemove(GLFWwindow *window) {

}

void mouseup(GLFWwindow *window) {

}

void mouseclick(GLFWwindow *window) {

    auto cameraRay = camera->getRay(glm::vec2(
            2 * mouseX / renderTarget->getWindowWidth() - 1.0f,
            1.0f - 2 * mouseY / renderTarget->getWindowHeight()));
    std::vector<glm::vec3> worldPositions;
    if (cameraRay.intersectObject(terrain.get(), worldPositions)) {
        glm::vec3 objectPosition = renderbox::dehomogenize(
                glm::inverse(terrain->getWorldMatrix())
                * glm::vec4(worldPositions[0] - glm::vec3(0.5f), 1.0f));

        renderbox::VoxelGeometry *voxelGeometry = (renderbox::VoxelGeometry *) terrain->getGeometry().get();

        voxelGeometry->brush(objectPosition, 8, 0.4f, isolevel);

        voxelGeometry->updateGeometryByMarchingCube(isolevel);
    }

}

void mousedrag(GLFWwindow *window) {

}

void mousedrop(GLFWwindow *window) {

}

void windowSizeCallback(GLFWwindow *window, int width, int height) {
    camera->setPerspective(glm::radians(45.0f), (float) renderTarget->getWindowWidth() / (float) renderTarget->getWindowHeight());
}

int keyMods = 0;

void keyCallback(GLFWwindow *window, int key, int scanCode, int action, int mods) {
    keyMods = mods;
    if (key == GLFW_KEY_Q) {
        if (action == GLFW_PRESS) {
            cameraAngularVelocity -= 180.f;
        } else if (action == GLFW_RELEASE) {
            cameraAngularVelocity += 180.f;
        }
    } else if (key == GLFW_KEY_E) {
        if (action == GLFW_PRESS) {
            cameraAngularVelocity += 180.f;
        } else if (action == GLFW_RELEASE) {
            cameraAngularVelocity -= 180.f;
        }
    } else if (key == GLFW_KEY_W) {
        if (action == GLFW_PRESS) {
            cameraVelocity[1] += 50.0f;
        } else if (action == GLFW_RELEASE) {
            cameraVelocity[1] -= 50.0f;
        }
    } else if (key == GLFW_KEY_S) {
        if (action == GLFW_PRESS) {
            cameraVelocity[1] -= 50.0f;
        } else if (action == GLFW_RELEASE) {
            cameraVelocity[1] += 50.0f;
        }
    } else if (key == GLFW_KEY_D) {
        if (action == GLFW_PRESS) {
            cameraVelocity[0] += 50.0f;
        } else if (action == GLFW_RELEASE) {
            cameraVelocity[0] -= 50.0f;
        }
    } else if (key == GLFW_KEY_A) {
        if (action == GLFW_PRESS) {
            cameraVelocity[0] -= 50.0f;
        } else if (action == GLFW_RELEASE) {
            cameraVelocity[0] += 50.0f;
        }
    }
}

void cursorPosCallback(GLFWwindow *window, double x, double y) {
    mouseX = x;
    mouseY = y;
    mousemove(window);
    if (startMouseX != -1 && startMouseY != -1
        && (fabs(startMouseX - mouseX) >= 5
            || fabs(startMouseY - mouseY) >= 5)) {
        mousedrag(window);
    }
}

void scrollCallback(GLFWwindow *window, double deltaX, double deltaY) {
    auto cameraDirection = camera->getRay(renderbox::vec2()).getDirection();
    auto forward = glm::normalize(glm::vec3(cameraDirection.x, cameraDirection.y, 0));
    auto right = glm::normalize(glm::vec3(cameraDirection.y, - cameraDirection.x, 0));
    if (keyMods & GLFW_MOD_ALT) {
        cameraAngle[1] += (float) - deltaY;
        return;
    }
    cameraRig->translate((forward * (float) deltaY - right * (float) deltaX)
                         * cameraDistance * 0.01f);
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            startMouseX = mouseX;
            startMouseY = mouseY;
            mousedown(window);
        }
        if (action == GLFW_RELEASE) {
            mouseup(window);
            if (fabs(startMouseX - mouseX) >= 5
                && fabs(startMouseY - mouseY) >= 5) {
                mousedrop(window);
            } else {
                mouseclick(window);
            }
            startMouseX = -1;
            startMouseY = -1;
        }
    }
}

void zoomCallback(GLFWwindow *window, double magnification) {
    cameraDistance /= (1 + magnification);
    camera->setTranslation(glm::vec3(0, 0, cameraDistance));
}

void rotateCallback(GLFWwindow *window, double rotation) {
    cameraAngle[0] += - rotation;
}

int main(int argc, char **argv) {

    renderer.reset(new renderbox::OpenGLRenderer());
    renderTarget.reset(new renderbox::GLFWOpenGLRenderTarget());
    GLFWwindow *window = renderTarget->getWindow();

    // Callbacks

    glfwSetWindowSizeCallback(window, windowSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetZoomCallback(window, zoomCallback);
    glfwSetRotateCallback(window, rotateCallback);

    // Render loop

    double lastTime = glfwGetTime();
    int nFrames = 0;

    init();
    while (!glfwWindowShouldClose(window)) {

        double currentTime = glfwGetTime();
        ++nFrames;
        if (currentTime - lastTime >= 1.0){
            fprintf(stdout, "%.1f ms/frame\t%d fps\n", 1000.0 / double(nFrames), nFrames);
            nFrames = 0;
            lastTime = currentTime;
        }

        glfwPollEvents();

        update();

        glfwSwapBuffers(window);

    }
    uninit();

    // End

    glfwTerminate();

    return 0;

}

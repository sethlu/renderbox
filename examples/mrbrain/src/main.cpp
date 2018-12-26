#include <iostream>
#include <fstream>

#include <glm/glm.hpp>

#define RENDERBOX_USE_OPENGL
#define RENDERBOX_USE_GLFW
#include "renderbox.h"

std::shared_ptr<renderbox::Scene> scene;
std::shared_ptr<renderbox::PerspectiveCamera> camera;
std::unique_ptr<renderbox::OpenGLRenderer> renderer;
std::unique_ptr<renderbox::GLFWOpenGLRenderTarget> renderTarget;

std::shared_ptr<renderbox::Object> cameraRig;
std::shared_ptr<renderbox::Mesh> brain;

float cameraDistance = 80.0f;
float cameraAngularVelocity = 0.0f;
float cameraAngle[] = {90.0f, 1.0f};
float cameraVelocity[] = {0.0f, 0.0f}; // Relative to the camera forward direction

float lastTime = (float) glfwGetTime();
double mouseX = -1, mouseY = -1, startMouseX = -1, startMouseY = -1;

float isolevel = 0.5f;
const int steps = 2;

void init() {

    // Scene
    scene = std::make_shared<renderbox::Scene>();
    scene->setAmbientColor(glm::vec3(0.05f));

    // Voxel brain
    auto voxelGeometry = std::make_shared<renderbox::VoxelGeometry>();

    for (int z = 1, _z = 0; z <= 109; z += steps, ++_z) {
        const char *filename = ("MRBrain/MRbrain." + std::to_string(z)).c_str();
        std::cout << "Loading " << filename << "\n";
        unsigned short buffer[256][256];

        std::ifstream in;
        in.open(filename);
        if (in.is_open()) {
            in.read((char *) *buffer, sizeof buffer);
            in.close();
        } else {
            throw 1;
        }

        for (int x = 0, _x = 0; x < 256; x += steps, ++_x) {
            for (int y = 0, _y = 0; y < 256; y += steps, ++_y) {
                float avg = 0;
                for (int dx = 0; dx < steps; ++dx)
                    for (int dy = 0; dy < steps; ++dy)
                        avg += buffer[x + dx][y + dy];
                avg /= (steps * steps);
                voxelGeometry->setOccupancy(_x, _y, _z, avg / 65536);
            }
        }
    }

    std::cout << "isolevel " << isolevel << "\n";
    voxelGeometry->updateGeometryByMarchingCube(isolevel); // Refresh geometry
    brain = std::make_shared<renderbox::Mesh>(voxelGeometry,
                                              std::make_shared<renderbox::MeshLambertMaterial>());
    brain->setScale(glm::vec3(1, 1, 2));
    brain->setTranslation(glm::vec3(-256 / 2 / steps, -256 / 2 / steps, -109 / steps));
    scene->addChild(brain);

    // Camera
    camera = std::make_shared<renderbox::PerspectiveCamera>(glm::radians(45.0f),
        (float) renderTarget->getWindowWidth() / (float) renderTarget->getWindowHeight());
    camera->setTranslation(glm::vec3(0, 0, cameraDistance));
    cameraRig = std::make_shared<renderbox::Object>();
    cameraRig->addChild(camera);
    cameraRig->rotate(glm::vec3(1.0f, 0, 0), glm::radians(cameraAngle[1]));

    auto pointLight = std::make_shared<renderbox::PointLight>(glm::vec3(3000.0f));
    pointLight->setTranslation(glm::vec3(0, 0, 400));
    scene->addChild(pointLight);

}

double mouseLastSync = glfwGetTime();

void update() {

    float currentTime = (float) glfwGetTime();
    float deltaTime = currentTime - lastTime;

    // Camera

    auto cameraDirection = camera->getRay(renderbox::vec2()).getDirection();
    auto forward = glm::normalize(glm::vec3(cameraDirection.x, cameraDirection.y, 0));
    auto right = glm::normalize(glm::vec3(cameraDirection.y, - cameraDirection.x, 0));
    cameraRig->translate(glm::vec3(right * cameraVelocity[0] + forward * cameraVelocity[1])
                         * deltaTime * cameraDistance * 0.01f);
    cameraRig->clearRotation();
    cameraAngle[0] += cameraAngularVelocity * deltaTime;
    if (cameraAngle[1] < 5.0f) cameraAngle[1] = 5.0f;
    else if (cameraAngle[1] > 85.0f) cameraAngle[1] = 85.0f;
    cameraRig->rotate(glm::vec3(1.0f, 0, 0), glm::radians(cameraAngle[1]));
    cameraRig->rotate(glm::vec3(0, 0, 1.0f), glm::radians(cameraAngle[0]));

    // Render

    renderer->render(scene.get(), camera.get(), renderTarget.get());

    lastTime = currentTime;

}

void uninit() {

}

void windowSizeCallback(GLFWwindow *window, int width, int height) {
    camera->setPerspective(glm::radians(45.0f),
            (float) renderTarget->getWindowWidth() / (float) renderTarget->getWindowHeight());
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
    } else if (key == GLFW_KEY_EQUAL) {
        if (action == GLFW_RELEASE) {
            isolevel += 0.01f;
            std::cout << "isolevel " << isolevel << "\n";
            ((renderbox::VoxelGeometry *) brain->getGeometry().get())->updateGeometryByMarchingCube(isolevel, true);
        }
    } else if (key == GLFW_KEY_MINUS) {
        if (action == GLFW_RELEASE) {
            isolevel -= 0.01f;
            std::cout << "isolevel " << isolevel << "\n";
            ((renderbox::VoxelGeometry *) brain->getGeometry().get())->updateGeometryByMarchingCube(isolevel, true);
        }
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
    glfwSetScrollCallback(window, scrollCallback);
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

        // Swap buffers
        glfwSwapBuffers(window);

    }
    uninit();

    // End

    glfwTerminate();

    return 0;

}

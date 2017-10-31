#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include "renderbox.h"

renderbox::Scene *scene;
renderbox::PerspectiveCamera *camera;
renderbox::OpenGLGLFWRenderer *renderer;

renderbox::Object *cameraRig;
renderbox::Mesh *terrain;
renderbox::Mesh *testCube;

float cameraDistance = 80.0f;
float cameraAngularVelocity = 0.0f;
float cameraAngle[] = {0.0f, 80.0f};
float cameraVelocity[] = {0.0f, 0.0f}; // Relative to the camera forward direction

float lastTime = (float) glfwGetTime();
double mouseX = -1, mouseY = -1, startMouseX = -1, startMouseY = -1;

float isolevel = 0.5f;
const int steps = 6;

void init() {

    // Scene
    scene = new renderbox::Scene();
    scene->setAmbientColor(glm::vec3(0.05f));

    // Voxel terrain
    auto *voxelGeometry = new renderbox::VoxelGeometry();

    for (int z = 1, _z = 0; z <= 109; z += steps, ++_z) {
        const char *filename = renderbox::copyString("MRBrain/MRbrain." + std::to_string(z));
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
    terrain = new renderbox::Mesh(voxelGeometry,
                                  new renderbox::GLSLShaderMaterial(renderbox::readFile("shaders/brain_vert.glsl"),
                                                                    renderbox::readFile("shaders/brain_frag.glsl")));
    terrain->setScale(glm::vec3(1, 1, 2));
    terrain->setTranslation(glm::vec3(-256 / 2 / steps, -256 / 2 / steps, -109 / steps));
    renderer->loadObject(terrain);
    scene->addChild(terrain);

    // Test cube
    testCube = new renderbox::Mesh(new renderbox::BoxGeometry(0.5f, 0.5f, 0.5f),
                                   new renderbox::MeshLambertMaterial(glm::vec3(1.0f, 0, 0)));
    renderer->loadObject(testCube);
    scene->addChild(testCube);

    // Camera
    camera = new renderbox::PerspectiveCamera(glm::radians(45.0f), (float) renderer->getWindowWidth() / (float) renderer->getWindowHeight());
    camera->setTranslation(glm::vec3(0, 0, cameraDistance));
    cameraRig = new renderbox::Object();
    cameraRig->addChild(camera);
    cameraRig->rotate(glm::vec3(1.0f, 0, 0), glm::radians(cameraAngle[1]));

}

double mouseLastSync = glfwGetTime();

void update() {

    float currentTime = (float) glfwGetTime();
    float deltaTime = currentTime - lastTime;

    // Terrain

    if (currentTime - mouseLastSync > 0.04f
        && startMouseX != -1 && startMouseY != -1) {

        renderbox::Ray *cameraRay = camera->getRay(glm::vec2(2 * mouseX / renderer->getWindowWidth() - 1.0f,
                                                             1.0f - 2 * mouseY / renderer->getWindowHeight()));
        std::vector<glm::vec3> worldPositions;
        if (cameraRay->intersectObject(terrain, worldPositions)) {
            glm::vec3 objectPosition = floor(renderbox::dehomogenize(
                    glm::inverse(terrain->getWorldMatrix())
                    * glm::vec4(worldPositions[0] + glm::vec3(0.5f), 1.0f)));

            renderbox::VoxelGeometry *terrainGeometry = (renderbox::VoxelGeometry *) terrain->getGeometry();

            terrainGeometry->brush(objectPosition, 8, 0.4f, isolevel);

            terrainGeometry->updateGeometryByMarchingCube(isolevel);
            renderer->loadObject(terrain);
        }

        mouseLastSync = currentTime;
    }

    // Test cube

    testCube->visible = false;
    if (mouseX >= 0 && mouseX <= renderer->getWindowWidth() && mouseY >= 0 && mouseY <= renderer->getWindowHeight()) {
        renderbox::Ray *cameraRay = camera->getRay(glm::vec2(2 * mouseX / renderer->getWindowWidth() - 1.0f,
                                                             1.0f - 2 * mouseY / renderer->getWindowHeight()));
        std::vector<glm::vec3> worldPositions;
        if (cameraRay->intersectObject(terrain, worldPositions)) {
            glm::vec3 testLocation = worldPositions[0];
            testCube->visible = true;
            testCube->setTranslation(testLocation);
        }
    }

    // Camera

    glm::vec3 cameraDirection = camera->getRay()->getDirection();
    glm::vec3 forward = glm::normalize(glm::vec3(cameraDirection.x, cameraDirection.y, 0));
    glm::vec3 right = glm::normalize(glm::vec3(cameraDirection.y, - cameraDirection.x, 0));
    cameraRig->translate(glm::vec3(right * cameraVelocity[0] + forward * cameraVelocity[1])
                         * deltaTime * cameraDistance * 0.01f);
    cameraRig->clearRotation();
    cameraAngle[0] += cameraAngularVelocity * deltaTime;
    if (cameraAngle[1] < 5.0f) cameraAngle[1] = 5.0f;
    else if (cameraAngle[1] > 85.0f) cameraAngle[1] = 85.0f;
    cameraRig->rotate(glm::vec3(1.0f, 0, 0), glm::radians(cameraAngle[1]));
    cameraRig->rotate(glm::vec3(0, 0, 1.0f), glm::radians(cameraAngle[0]));

    // Render

    renderer->render(scene, camera);

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

    renderbox::Ray *cameraRay = camera->getRay(glm::vec2(2 * mouseX / renderer->getWindowWidth() - 1.0f,
                                                         1.0f - 2 * mouseY / renderer->getWindowHeight()));
    std::vector<glm::vec3> worldPositions;
    if (cameraRay->intersectObject(terrain, worldPositions)) {
        glm::vec3 objectPosition = renderbox::dehomogenize(
                glm::inverse(terrain->getWorldMatrix())
                * glm::vec4(worldPositions[0] - glm::vec3(0.5f), 1.0f));

        renderbox::VoxelGeometry *terrainGeometry = (renderbox::VoxelGeometry *) terrain->getGeometry();

        terrainGeometry->brush(objectPosition, 5, 0.4f, isolevel);

        terrainGeometry->updateGeometryByMarchingCube(isolevel);
        renderer->loadObject(terrain);
    }

}

void mousedrag(GLFWwindow *window) {

}

void mousedrop(GLFWwindow *window) {

}

void windowSizeCallback(GLFWwindow *window, int width, int height) {
    camera->setPerspective(glm::radians(45.0f), (float) renderer->getWindowWidth() / (float) renderer->getWindowHeight());
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
            ((renderbox::VoxelGeometry *) terrain->getGeometry())->updateGeometryByMarchingCube(isolevel, true);
            renderer->loadObject(terrain);
        }
    } else if (key == GLFW_KEY_MINUS) {
        if (action == GLFW_RELEASE) {
            isolevel -= 0.01f;
            std::cout << "isolevel " << isolevel << "\n";
            ((renderbox::VoxelGeometry *) terrain->getGeometry())->updateGeometryByMarchingCube(isolevel, true);
            renderer->loadObject(terrain);
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
    glm::vec3 cameraDirection = camera->getRay()->getDirection();
    glm::vec3 forward = glm::normalize(glm::vec3(cameraDirection.x, cameraDirection.y, 0));
    glm::vec3 right = glm::normalize(glm::vec3(cameraDirection.y, - cameraDirection.x, 0));
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

    renderer = new renderbox::OpenGLGLFWRenderer();
    GLFWwindow *window = renderer->getWindow();

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

        update();

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    uninit();

    // End

    glfwTerminate();

    return 0;

}

#include <iostream>
#include "renderbox/renderbox.h"

#define BUFFER_OFFSET(i) ((char*) nullptr + i);

renderbox::Scene *scene;
renderbox::PerspectiveCamera *camera;
renderbox::Renderer *renderer;

renderbox::Object *cameraRig;
renderbox::Mesh *terrain;
renderbox::Mesh *testCube;

double cameraDistance = 40;
double cameraAngle = glm::radians(-90.0f);
double cameraAngularVelocity = 0.0f;

double lastTime = glfwGetTime();
double mouseX = 0.0f, mouseY = 0.0f;

float isolevel = 0.5f;

void init() {

    // Scene
    scene = new renderbox::Scene();

    // Voxel terrain
    renderbox::VoxelGeometry *voxelGeometry = new renderbox::VoxelGeometry();
    for (int x = -32; x < 32; ++x) {
        for (int y = -32; y < 32; ++y) {
            voxelGeometry->setOccupancy(x, y, 0, 1.0f);
        }
    }
    voxelGeometry->updateGeometry(isolevel); // Refresh geometry
    terrain = new renderbox::Mesh(voxelGeometry, new renderbox::Material());
    renderer->loadObject(terrain);
    scene->addChild(terrain);

    // Test cube
    testCube = new renderbox::Mesh(new renderbox::BoxGeometry(1.0f, 1.0f, 1.0f), new renderbox::Material());
    renderer->loadObject(testCube);
    scene->addChild(testCube);

    // Camera
    camera = new renderbox::PerspectiveCamera(glm::radians(45.0f), (float) renderer->getWindowWidth() / (float) renderer->getWindowHeight());
    camera->setTranslation(glm::vec3(0, 0, cameraDistance));
    cameraRig = new renderbox::Object();
    cameraRig->addChild(camera);
    cameraRig->rotate(glm::vec3(1.0f, 0, 0), glm::radians(60.0f));

}

void update() {

    double currentTime = glfwGetTime();
    double deltaTime = currentTime - lastTime;

    // Terrain

    // Test cube

    renderbox::Ray *cameraRay = camera->getRay(glm::vec2(2 * mouseX / renderer->getWindowWidth() - 1.0f,
                                                         1.0f - 2 * mouseY / renderer->getWindowHeight()));
    std::vector<glm::vec3> worldPositions;
    testCube->visible = false;
    if (cameraRay->intersectObject(terrain, worldPositions)) {
        renderbox::VoxelGeometry *terrainGeometry = (renderbox::VoxelGeometry *) terrain->getGeometry();
        glm::vec3 testLocation = floor(worldPositions[0] + glm::vec3(0.5f));
        testCube->visible = true;
        testCube->setTranslation(testLocation);
    }

    // Camera

    cameraRig->rotate(glm::vec3(0, 0, 1.0f), cameraAngularVelocity * deltaTime);

    // Render

    renderer->render(scene, camera);

    lastTime = currentTime;

}

void windowSizeCallback(GLFWwindow *window, int width, int height) {
    camera->setPerspective(glm::radians(45.0f), (float) renderer->getWindowWidth() / (float) renderer->getWindowHeight());
}

void keyCallback(GLFWwindow *window, int key, int scanCode, int action, int mods) {
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        cameraAngularVelocity = -glm::radians(180.0f);
    } else if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        cameraAngularVelocity = glm::radians(180.0f);
    } else if ((key == GLFW_KEY_Q || key == GLFW_KEY_E) && action == GLFW_RELEASE) {
        cameraAngularVelocity = 0.0f;
    }
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {

            renderbox::Ray *cameraRay = camera->getRay(glm::vec2(2 * mouseX / renderer->getWindowWidth() - 1.0f,
                                                                 1.0f - 2 * mouseY / renderer->getWindowHeight()));
            std::vector<glm::vec3> worldPositions;
            if (cameraRay->intersectObject(terrain, worldPositions)) {
                glm::vec3 objectPosition = floor(renderbox::dehomogenize(
                        glm::inverse(terrain->getWorldMatrix())
                        * glm::vec4(worldPositions[0] + glm::vec3(0.5f), 1.0f)));

                renderbox::VoxelGeometry *terrainGeometry = (renderbox::VoxelGeometry *) terrain->getGeometry();

                terrainGeometry->setOccupancy(objectPosition + glm::vec3(-1, 0, 0), 1.0f);
                terrainGeometry->setOccupancy(objectPosition + glm::vec3(0, 0, 0), 1.0f);
                terrainGeometry->setOccupancy(objectPosition + glm::vec3(0, -1, 0), 1.0f);
                terrainGeometry->setOccupancy(objectPosition + glm::vec3(-1, -1, 0), 1.0f);
                terrainGeometry->setOccupancy(objectPosition + glm::vec3(-1, 0, -1), 1.0f);
                terrainGeometry->setOccupancy(objectPosition + glm::vec3(0, 0, -1), 1.0f);
                terrainGeometry->setOccupancy(objectPosition + glm::vec3(0, -1, -1), 1.0f);
                terrainGeometry->setOccupancy(objectPosition + glm::vec3(-1, -1, -1), 1.0f);

                terrainGeometry->updateGeometry(isolevel);
                renderer->loadObject(terrain);
            }

        }
    }
}

void cursorPosCallback(GLFWwindow *window, double x, double y) {
    mouseX = x;
    mouseY = y;
}

void scrollCallback(GLFWwindow *window, double deltaX, double deltaY) {

}

void uninit() {

}

int main(int argc, char **argv) {

    renderer = new renderbox::Renderer();
    GLFWwindow *window = renderer->getWindow();

    // Callbacks

    glfwSetWindowSizeCallback(window, windowSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // Drawing loop

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

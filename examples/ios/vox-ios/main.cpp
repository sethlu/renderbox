#include <glm/glm.hpp>
#include "renderbox.h"


std::shared_ptr<renderbox::Scene> scene;
std::shared_ptr<renderbox::Camera> camera;
std::unique_ptr<renderbox::OpenGLSDLRenderer> renderer;

std::shared_ptr<renderbox::Object> cameraRig;
std::shared_ptr<renderbox::Object> cube;

std::shared_ptr<renderbox::PointLight> pointLight;

void init() {
	
	scene = std::make_shared<renderbox::Scene>();
	
	cube = std::make_shared<renderbox::Object>(std::make_shared<renderbox::BoxGeometry>(3, 3, 3),
											   std::make_shared<renderbox::MeshLambertMaterial>(glm::vec3(1, 0, 0)));
	scene->addChild(cube);
	
	camera = std::make_shared<renderbox::PerspectiveCamera>(glm::radians(45.0f),
															(float) renderer->getFramebufferWidth() / (float) renderer->getFramebufferHeight());
	camera->setTranslation(glm::vec3(0, 0, 20));
	
	cameraRig = std::make_shared<renderbox::Object>();
	cameraRig->addChild(camera);
	cameraRig->rotate(glm::vec3(1, 0, 0), glm::radians(45.0f));
	cameraRig->rotate(glm::vec3(0, 0, 1), glm::radians(45.0f));
	
	pointLight = std::make_shared<renderbox::PointLight>(glm::vec3(20));
	pointLight->setTranslation(glm::vec3(0, 0, 6.0f));
	scene->addChild(pointLight);
	
}

void render() {
	
	renderer->render(scene.get(), camera.get());
	
	SDL_GL_SwapWindow(renderer->getWindow());
	
}

int main(int argc, char **argv) {
	
	renderer.reset(new renderbox::OpenGLSDLRenderer());
	
	// Render loop
	
	SDL_Event event;
	bool done = false;
	
	init();
	while (!done) {

        while (SDL_PollEvent(&event)) {
			switch (event.type) {
                case SDL_MOUSEMOTION: {

					renderbox::Ray *ray = camera->getRay(glm::vec2(0.0f, 0.0f));
					glm::vec3 up = glm::vec3(0, 0, 1.0f);
					glm::vec3 right = glm::normalize(glm::cross(ray->getDirection(), up));

					cube->rotate(up, (float) event.motion.xrel / 100);
					cube->rotate(right, (float) event.motion.yrel / 100);

					break;
				}
				case SDL_QUIT:
					done = true;
					break;
			}
		}
		
		render();
		
		SDL_Delay(1);
		
	}
	
	// End
	
	SDL_Quit();
	
	return 0;
}

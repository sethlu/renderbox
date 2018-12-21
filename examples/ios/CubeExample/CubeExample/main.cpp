#include <iostream>

#define RENDERBOX_USE_OPENGL
#define RENDERBOX_USE_SDL
#include "renderbox.h"


std::shared_ptr<renderbox::Scene> scene;
std::shared_ptr<renderbox::PerspectiveCamera> camera;
std::unique_ptr<renderbox::OpenGLRenderer> renderer;
std::unique_ptr<renderbox::SDLOpenGLRenderTarget> renderTarget;

std::shared_ptr<renderbox::Object> cameraRig;
std::shared_ptr<renderbox::Object> cube;

std::shared_ptr<renderbox::PointLight> pointLight;

void init() {
	
	scene = std::make_shared<renderbox::Scene>();
	
	cube = std::make_shared<renderbox::Object>(std::make_shared<renderbox::BoxGeometry>(3, 3, 3),
											   std::make_shared<renderbox::MeshLambertMaterial>(renderbox::vec3(1, 0, 0)));
	scene->addChild(cube);
	
	camera = std::make_shared<renderbox::PerspectiveCamera>(glm::radians(45.0f),
															(float) renderTarget->getFramebufferWidth() / (float) renderTarget->getFramebufferHeight());
	camera->setTranslation(renderbox::vec3(0, 0, 20));
	
	cameraRig = std::make_shared<renderbox::Object>();
	cameraRig->addChild(camera);
	cameraRig->rotate(renderbox::vec3(1, 0, 0), glm::radians(45.0f));
	cameraRig->rotate(renderbox::vec3(0, 0, 1), glm::radians(45.0f));
	
	pointLight = std::make_shared<renderbox::PointLight>(renderbox::vec3(20));
	pointLight->setTranslation(renderbox::vec3(0, 0, 6.0f));
	scene->addChild(pointLight);
	
}

void render() {
	
	renderer->render(scene.get(), camera.get(), renderTarget.get());
	
	SDL_GL_SwapWindow(renderTarget->getWindow());
	
}

int main(int argc, char **argv) {
	
	renderer.reset(new renderbox::OpenGLRenderer());
    renderTarget.reset(new renderbox::SDLOpenGLRenderTarget());
	
	// Render loop
	
	SDL_Event event;
	bool done = false;
	
	init();
	while (!done) {

        while (SDL_PollEvent(&event)) {
			switch (event.type) {
                case SDL_MOUSEMOTION: {

					auto ray = camera->getRay(renderbox::vec2(0.0f, 0.0f));
					auto up = renderbox::vec3(0, 0, 1.0f);
					auto right = glm::normalize(glm::cross(ray.getDirection(), up));

					cube->rotate(up, (float) event.motion.xrel / 100);
					cube->rotate(right, (float) event.motion.yrel / 100);
					
					break;
				}
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                        camera->setPerspective(glm::radians(45.0f), (float) renderTarget->getFramebufferWidth() / (float) renderTarget->getFramebufferHeight());
                    }
                    break;
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

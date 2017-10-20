#include "SDL.h"
#include "renderbox.h"


renderbox::Scene *scene;
renderbox::Object *cameraRig;
renderbox::Camera *camera;
renderbox::OpenGLSDLRenderer *renderer;
renderbox::Object *cube;

void init() {
	
	scene = new renderbox::Scene();
	
    cube = new renderbox::Object(new renderbox::BoxGeometry(3, 3, 3), new renderbox::MeshLambertMaterial(glm::vec3(1, 0, 0)));
	renderer->loadObject(cube);
	scene->addChild(cube);
	
	camera = new renderbox::PerspectiveCamera(glm::radians(45.0f), (float) renderer->getFramebufferWidth() / (float) renderer->getFramebufferHeight());
	camera->setTranslation(glm::vec3(0, 0, 20));
	
	cameraRig = new renderbox::Object;
	cameraRig->addChild(camera);
	cameraRig->rotate(glm::vec3(1, 0, 0), glm::radians(45.0f));
	cameraRig->rotate(glm::vec3(0, 0, 1), glm::radians(45.0f));
	
}

void render() {
	
	renderer->render(scene, camera);
	
	SDL_GL_SwapWindow(renderer->getWindow());
	
}

int main(int argc, char **argv) {
	
	renderer = new renderbox::OpenGLSDLRenderer();
	
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
	
	delete scene;
	delete camera;
	delete renderer;
	
	SDL_Quit();
	
	return 0;
}

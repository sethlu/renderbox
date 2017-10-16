#include "SDL.h"
#include "../../renderbox/renderbox.h"


renderbox::Scene *scene;
renderbox::Object *cameraRig;
renderbox::Camera *camera;
renderbox::OpenGLSDLRenderer *renderer;

void init() {
	
	scene = new renderbox::Scene();
	
	renderbox::Object *cube = new renderbox::Object(new renderbox::BoxGeometry(2, 2, 2), new renderbox::MeshLambertMaterial(glm::vec3(1, 0, 0)));
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
	
	glClearColor(0, 0, 1, 1);
	
	// Render loop
	
	SDL_Event event;
	int done = 0;
	
	init();
	while (!done) {
		
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				done = 1;
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

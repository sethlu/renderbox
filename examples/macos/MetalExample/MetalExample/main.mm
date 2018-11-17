
#define OPENGL_COMPANION

#ifdef OPENGL_COMPANION
#define RENDERBOX_USE_OPENGL
#endif //OPENGL_COMPANION

#include <iostream>
#define RENDERBOX_USE_METAL
#define RENDERBOX_USE_SDL
#include <renderbox.h>


std::shared_ptr<renderbox::Scene> scene;
std::shared_ptr<renderbox::Camera> camera;

std::unique_ptr<renderbox::MetalRenderer> renderer0;
std::unique_ptr<renderbox::SDLMetalRenderTarget> renderTarget0;

#ifdef OPENGL_COMPANION
std::unique_ptr<renderbox::OpenGLRenderer> renderer1;
std::unique_ptr<renderbox::SDLOpenGLRenderTarget> renderTarget1;
#endif //OPENGL_COMPANION

std::shared_ptr<renderbox::Object> cameraRig;
std::shared_ptr<renderbox::Object> cube;

std::shared_ptr<renderbox::PointLight> pointLight;

void init() {
	
	scene = std::make_shared<renderbox::Scene>();

    cube = std::make_shared<renderbox::Object>(std::make_shared<renderbox::BoxGeometry>(3, 3, 3),
                                               std::make_shared<renderbox::MeshLambertMaterial>(renderbox::vec3(1, 0, 0)));
	scene->addChild(cube);

	camera = std::make_shared<renderbox::PerspectiveCamera>(glm::radians(45.0f),
															(float) renderTarget0->getFramebufferWidth() / (float) renderTarget0->getFramebufferHeight());
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
	
    renderer0->render(scene.get(), camera.get(), renderTarget0.get());

#ifdef OPENGL_COMPANION
    renderer1->render(scene.get(), camera.get(), renderTarget1.get());
    SDL_GL_SwapWindow(renderTarget1->getWindow());
#endif //OPENGL_COMPANION

}

int main(int argc, char **argv) {
	
#ifdef OPENGL_COMPANION
    renderer1.reset(new renderbox::OpenGLRenderer());
    renderTarget1.reset(new renderbox::SDLOpenGLRenderTarget());
#endif //OPENGL_COMPANION
    
    renderer0.reset(new renderbox::MetalRenderer());
    renderTarget0.reset(new renderbox::SDLMetalRenderTarget());

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

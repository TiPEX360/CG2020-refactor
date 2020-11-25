#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include "Camera.h"
#include "Scene.h"
#include "Rasterizer.h"
#include "Renderer.h"

#define WIDTH 640
#define HEIGHT 480

void draw(DrawingWindow &window, Scene scene) {
	window.clearPixels();
	

}

void update(DrawingWindow &window, Uint32 deltaTime, Scene scene) {
	// Function for performing animation (shifting artifacts or moving the camera)
}

void handleEvent(SDL_Event event, DrawingWindow &window) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT) std::cout << "LEFT" << std::endl;
		else if (event.key.keysym.sym == SDLK_RIGHT) std::cout << "RIGHT" << std::endl;
		else if (event.key.keysym.sym == SDLK_UP) std::cout << "UP" << std::endl;
		else if (event.key.keysym.sym == SDLK_DOWN) std::cout << "DOWN" << std::endl;
	} else if (event.type == SDL_MOUSEBUTTONDOWN) window.savePPM("output.ppm");
}

int main(int argc, char *argv[]) {
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;

	//Scene setup
	Scene scene(Camera(glm::vec3(0.0f, 0.0f, 2.0f), glm::mat3(1.0f), 2.0f), "cornell.obj", 0.17f);
	Renderer r = Rasterizer(window, &scene);
	//Tick
	Uint32 prevTime = SDL_GetTicks();
	while (true) {
		Uint32 deltaTime = SDL_GetTicks() - prevTime;
		prevTime += deltaTime;

		//Handle events
		if (window.pollForInputEvents(event)) handleEvent(event, window);
		
		//Update scene
		update(window, deltaTime, scene);

		//Draw frame
		draw(window, scene);
		window.renderFrame();
	}
}

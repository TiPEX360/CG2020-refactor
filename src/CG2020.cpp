#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>

#define WIDTH 320
#define HEIGHT 240

void draw(DrawingWindow &window, Uint32 deltaTime) {
	window.clearPixels();
	std::cout << deltaTime << std::endl;
}

void update(DrawingWindow &window) {
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

	//Tick
	Uint32 prevTime = SDL_GetTicks();
	while (true) {
		Uint32 deltaTime = SDL_GetTicks() - prevTime;
		prevTime += deltaTime;

		//Handle events
		if (window.pollForInputEvents(event)) handleEvent(event, window);
		
		//Update scene
		update(window);

		//Draw frame
		draw(window, deltaTime);
		window.renderFrame();
	}
}

#include "sdl-leap.h"
#include <iostream>

#undef main

using namespace leap;

int main() {
	SDL_Init(SDL_INIT_EVERYTHING);
	render::Window window("Testing Leap!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_RESIZABLE);
	render::Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);
	event::Event event;
	input::keys::KeyMap keys;
	auto img = image::load("test.png");
	image::convert(renderer, img);
	bool quit = false;
	while (!quit) {
		event.poll();
		switch (event->type) {
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
		default:
			break;
		}
		renderer.set_color(0, 0, 0, 255);
		renderer.clear();
	}
	return 0;
}
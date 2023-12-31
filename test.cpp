#include "sdl-leap.h"
#include <iostream>

#undef main
#define WORKSPACE "D:/workspace/Cpp/Library/sdl-leap/"
using namespace leap;
using namespace widget;

const color::Color black = { 0,0,0 },
white = { 255,255,255 },
grey = { 200,200,200 },
light_blue{ 0, 108, 190 },
dark_blue{0,10,80};

int main(int argc, char **argv) {
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	auto window = pointer::make_window("Leap", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_RESIZABLE);
	auto renderer = pointer::make_renderer(*window, 0, SDL_RENDERER_ACCELERATED);
	renderer->set_logical_size(1920, 1080);

	event::Event event;
	auto mouse = pointer::make_mouse();
	auto key_map = pointer::make_key_map();

	auto font_fam = pointer::make_font_family(WORKSPACE "/test/arial.ttf");

	text_box::TextBox text{
		text_box::create_single_formatter(font_fam->at(40), white),
		text_box::create_convertor()
	};

	text.push(*renderer, "Hello, world!", { 100,100 });


	pos::Rect range = { 500, 300, 600, 300 };

	auto style = button::make_style(
		button::make_style_bit(*renderer, dark_blue, grey, font_fam->at(30)->render_blended("Dogs!", white), range.size(), 3),
		button::make_style_bit(*renderer, light_blue, grey, font_fam->at(30)->render_blended("Cats!", white), range.size(), 3),
		button::make_style_bit(*renderer, light_blue, white, font_fam->at(40)->render_blended("Doggies!", black), range.size(), 3)
	);

	button::Button button(button::mouse::make_detector(mouse), button::mouse::make_clicker(mouse), button::make_drawer(style), range);

	pos::Rect ip_range = { 1200, 500, 500, 200 };
	auto ip_style = input_box::make_style(
		input_box::make_style_bit(*renderer, dark_blue, grey, font_fam->at(30)->render_blended("Input here!", white), ip_range.size(), 3),
		input_box::make_style_bit(*renderer, light_blue, grey, nullptr, ip_range.size(), 3)
	);
	input_box::InputBox input_box{
		input_box::mouse::make_focus_changer(mouse),
			input_box::make_cursor_mover(key_map),
			input_box::make_inputer(key_map),
			input_box::make_back_drawer(ip_style),
			input_box::make_text_drawer(font_fam->at(30), white),
			ip_range
	};


	bool quit = false;
	while (!quit) {
		event.poll();
		switch (event->type) {
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			mouse->button(event->button);
			break;
		case SDL_MOUSEMOTION:
			mouse->motion(event->motion);
			break;
		case SDL_KEYDOWN:
			key_map->key_down(event->key.keysym.sym);
			break;
		case SDL_KEYUP:
			key_map->key_up(event->key.keysym.sym);
		default:
			break;
		}

		renderer->set_color(black);
		renderer->clear();

		text.update();
		text.draw(*renderer);

		button.update();
		button.draw(*renderer);

		input_box.update();
		input_box.draw(*renderer);

		renderer->present();
	}

	return 0;
}
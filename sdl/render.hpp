#pragma once

#include "const.h"
#include "position.hpp"
#include "except.hpp"

namespace leap {
	namespace render {
		using pos::IPoint;
		using pos::IRect;

		class Window {
			SDL_Window *window_;

		public:
			Window(const char *title, int x, int y, int w, int h, Uint32 flags, bool shaped=false) noexcept {
				if (shaped)
					window_ = SDL_CreateShapedWindow(title, x, y, w, h, flags);
				else
					window_ = SDL_CreateWindow(title, x, y, w, h, flags);
			}

			explicit Window(SDL_Window *window) : window_(window) { }

			~Window() noexcept {
				SDL_DestroyWindow(window_);
			}

			Window() = delete;
			Window(const Window &) = delete;

			SDL_Window *get() const noexcept {
				return window_;
			}
		};

		class Renderer {
			SDL_Renderer *renderer_;

		public:
			Renderer(const Window &window, int index, Uint32 flags) noexcept {
				renderer_ = SDL_CreateRenderer(window.get(), index, flags);
			}

			explicit Renderer(SDL_Renderer *renderer) noexcept : renderer_(renderer) { }

			~Renderer() noexcept {
				SDL_DestroyRenderer(renderer_);
			}

			Renderer() = delete;
			Renderer(const Renderer &) = delete;

			SDL_Renderer *get() const noexcept {
				return renderer_;
			}

			SDL_Texture *convert(SDL_Surface *surface) const {
				auto result = SDL_CreateTextureFromSurface(renderer_, surface);
				if (result)
					return result;
				except::throw_exc();
			}

			void set_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const {
				if (SDL_SetRenderDrawColor(renderer_, r, g, b, a))
					except::throw_exc();
			}

			void set_color(const SDL_Color &color) const noexcept {
				set_color(color.r, color.g, color.b, color.a);
			}

			void set_logical_size(int w, int h) const {
				if (SDL_RenderSetLogicalSize(renderer_, w, h))
					except::throw_exc();
			}

			void set_logical_size(const IPoint &size) const noexcept {
				set_logical_size(size.x, size.y);
			}

			void copy(SDL_Texture* texture, const SDL_Rect* src, const SDL_Rect* dst) const {
				if (SDL_RenderCopy(renderer_, texture, src, dst))
					except::throw_exc();
			}

			void copy(SDL_Texture* texture, const IRect& src, const IRect& dst) const {
				copy(texture, &src, &dst);
			}

			void draw_point(int x, int y) const {
				if (SDL_RenderDrawPoint(renderer_, x, y))
					except::throw_exc();
			}

			void draw_point(const IPoint &point) const {
				draw_point(point.x, point.y);
			}

			void draw_line(int x1, int y1, int x2, int y2) const {
				if (SDL_RenderDrawLine(renderer_, x1, y1, x2, y2))
					except::throw_exc();
			}

			void draw_line(const IPoint &from, const IPoint &to) const {
				draw_line(from.x, from.y, to.x, to.y);
			}

			void draw_rect(const SDL_Rect *rect) const {
				if (SDL_RenderDrawRect(renderer_, rect))
					except::throw_exc();
			}

			void draw_rect(int x, int y, int w, int h) const {
				const SDL_Rect rect{x, y, w, h};
				draw_rect(&rect);
			}

			void draw_rect(const IRect &rect) const {
				draw_rect(&rect);
			}

			void clear() const {
				if (SDL_RenderClear(renderer_))
					except::throw_exc();
			}

			void present() const noexcept {
				SDL_RenderPresent(renderer_);
			}
		};
	}
}

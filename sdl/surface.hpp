#pragma once

#include "const.h"
#include "except.hpp"
#include "position.hpp"
#include "render.hpp"
#include "texture.hpp"

namespace leap {
	namespace surface {
		class Surface {
			SDL_Surface *surface_;

		public:
			explicit Surface(SDL_Surface *surface) noexcept : surface_(surface) {}

			Surface(const int width, const int height, const int depth = 32, const int r_mask = 0,
			        const int g_mask = 0, const int b_mask = 0, const int a_mask = 0) :
				surface_(SDL_CreateRGBSurface(0, width, height, depth, r_mask, g_mask, b_mask, a_mask)) {
				if (surface_ == nullptr)
					except::throw_exc();
			}

			Surface(const pos::IPoint &size, const int depth, const int r_mask = 0,
			        const int g_mask = 0, const int b_mask = 0, const int a_mask = 0):
				Surface(size.x, size.y, depth, r_mask, g_mask, b_mask, a_mask) { }

			explicit Surface(const pos::IPoint &size) :
				Surface(size, 32) { }

			Surface(const Surface &surface) :
				surface_(surface.copy()) { }

			~Surface() {
				SDL_FreeSurface(surface_);
			}

			Surface() = delete;


			bool operator==(const Surface &surface) const noexcept {
				return this == &surface;
			}

			SDL_Surface *operator->() const noexcept {
				return surface_;
			}

			operator SDL_Surface*() const noexcept {
				return surface_;
			}

			SDL_Surface *get() const noexcept {
				return surface_;
			}

			SDL_Surface *copy() const {
				SDL_Surface *surface = SDL_ConvertSurface(surface_, surface_->format, 0);
				if (surface == nullptr)
					except::throw_exc();
				return surface;
			}

			Uint32 map_rgb(Uint8 r, Uint8 g, Uint8 b) const noexcept {
				return SDL_MapRGB(surface_->format, r, g, b);
			}

			Uint32 map_rgb(const SDL_Color &color) const noexcept {
				return map_rgb(color.r, color.g, color.b);
			}

			void set_color_key(Uint32 rgb, SDL_bool flag = SDL_TRUE) const {
				SDL_SetColorKey(surface_, flag, rgb);
			}

			void set_color_key(const SDL_Color &color, SDL_bool flag = SDL_TRUE) const {
				set_color_key(map_rgb(color), flag);
			}

			void fill(const SDL_Color &color) const {
				SDL_FillRect(surface_, nullptr, map_rgb(color));
			}

			void fill(const SDL_Color &color, const SDL_Rect &dst_rect) const {
				SDL_FillRect(surface_, &dst_rect, map_rgb(color));
			}

			pos::IPoint get_size() const noexcept {
				return {surface_->w, surface_->h};
			}

			pos::IRect get_range() const noexcept {
				return {0, 0, surface_->w, surface_->h};
			}

			void blit(SDL_Surface *src, const SDL_Rect *src_rect, SDL_Rect *dst_rect) const noexcept {
				if (SDL_BlitSurface(src, src_rect, surface_, dst_rect))
					except::throw_exc();
			}

			void blit(const Surface &src, const pos::IRect &src_rect, pos::IRect &dst_rect) const {
				blit(src.get(), &src_rect, &dst_rect);
			}

			void blit(const Surface &src, pos::IRect &dst_rect) const {
				const auto src_rect = src.get_range();
				blit(src.get(), &src_rect, &dst_rect);
			}

			void blit(const Surface &src, const pos::IPoint &dst) const {
				const auto src_rect = src.get_range();
				pos::IRect dst_rect = src_rect + dst;
				blit(src.get(), &src_rect, &dst_rect);
			}
		};
	}
}

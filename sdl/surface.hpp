#pragma once

#include "const.h"
#include "except.hpp"
#include "position.hpp"
#include "render.hpp"
#include "texture.hpp"

namespace leap {
	namespace surface {
		class Surface {
			SDL_Surface* surface_;
		public:
			explicit Surface(SDL_Surface *surface) noexcept : surface_(surface) {}

			~Surface() {
				SDL_FreeSurface(surface_);
			}

			Surface() = delete;
			Surface(const Surface &) = delete;

			bool operator==(const Surface &surface) const noexcept{
				return this == &surface;
			}

			SDL_Surface *operator->() const noexcept {
				return surface_;
			}

			operator SDL_Surface *() const noexcept {
				return surface_;
			}

			SDL_Surface *get() const noexcept {
				return surface_;
			}

			Uint32 map_rgb(Uint8 r, Uint8 g, Uint8 b) const noexcept {
				return SDL_MapRGB(surface_->format, r, g, b);
			}

			Uint32 map_rgb(const SDL_Color &color) const noexcept {
				return map_rgb(color.r, color.g, color.b);
			}

			void set_color_key(Uint32 rgb, SDL_bool flag=SDL_TRUE) const {
				SDL_SetColorKey(surface_, flag, rgb);
			}

			void set_color_key(const SDL_Color &color, SDL_bool flag=SDL_TRUE) const {
				set_color_key(map_rgb(color), flag);
			}
		};
	}
}
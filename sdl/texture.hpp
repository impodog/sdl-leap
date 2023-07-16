#pragma once

#include "const.h"
#include "except.hpp"
#include "position.hpp"
#include "render.hpp"

namespace leap {
	namespace texture {
		class Texture {
			SDL_Texture* texture_;
		public:
			explicit Texture(SDL_Texture* texture) noexcept : texture_(texture) {}

			~Texture() noexcept {
				SDL_DestroyTexture(texture_);
			}

			Texture() = delete;
			Texture(const Texture&) = delete;

			bool operator==(const Texture &other) const noexcept {
				return this == &other;
			}

			operator SDL_Texture *() const noexcept {
				return texture_;
			}

			SDL_Texture *get() const noexcept {
				return texture_;
			}
		};
	}
}
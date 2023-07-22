#pragma once

#include "const.h"
#include "except.hpp"
#include "position.hpp"
#include "render.hpp"

namespace leap {
	namespace texture {
		class Texture {
			SDL_Texture *texture_;

		public:
			explicit Texture(SDL_Texture *texture) noexcept : texture_(texture) {}

			~Texture() noexcept {
				SDL_DestroyTexture(texture_);
			}

			Texture() = delete;
			Texture(const Texture &) = delete;

			bool operator==(const Texture &other) const noexcept {
				return this == &other;
			}

			operator SDL_Texture*() const noexcept {
				return texture_;
			}

			SDL_Texture *operator *() const noexcept {
				return texture_;
			}

			SDL_Texture *get() const noexcept {
				return texture_;
			}

			pos::IPoint query_size() const noexcept {
				int w, h;
				SDL_QueryTexture(texture_, nullptr, nullptr, &w, &h);
				return {w, h};
			}

			pos::IRect query_range() const noexcept {
				int w, h;
				SDL_QueryTexture(texture_, nullptr, nullptr, &w, &h);
				return {0, 0, w, h};
			}

			void copy_to(const render::Renderer &renderer, const pos::IPoint &dst) const {
				const auto range = query_range();
				renderer.copy(texture_, range, range + dst);
			}

			void copy_to(const render::Renderer &renderer, const pos::IRect &dst) const {
				renderer.copy(texture_, query_range(), dst);
			}

			void copy_to(const render::Renderer &renderer, const pos::IRect &src, const pos::IRect &dst) const {
				renderer.copy(texture_, src, dst);
			}
		};
	}
}

#pragma once

#include "const.h"
#include "render.hpp"
#include "texture.hpp"
#include "surface.hpp"
#include <memory>

namespace leap {
	namespace pointer {
		using WindowPtr = std::shared_ptr<render::Window>;
		using RendererPtr = std::shared_ptr<render::Renderer>;
		using TexturePtr = std::shared_ptr<texture::Texture>;
		using SurfacePtr = std::shared_ptr<surface::Surface>;

		template <typename... Types>
		inline WindowPtr make_window(Types&&... args) {
			return std::make_shared<render::Window>(std::forward<Types>(args)...);
		}

		template <typename... Types>
		inline RendererPtr make_renderer(Types&&... args) {
			return std::make_shared<render::Renderer>(std::forward<Types>(args)...);
		}

		template <typename... Types>
		inline TexturePtr make_texture(Types&&... args) {
			return std::make_shared<texture::Texture>(std::forward<Types>(args)...);
		}

		template <typename... Types>
		inline SurfacePtr make_surface(Types&&... args) {
			return std::make_shared<surface::Surface>(std::forward<Types>(args)...);
		}
	}
}

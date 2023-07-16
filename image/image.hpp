#pragma once

#include "../sdl/sdl_packs.h"
#include "SDL_image.h"

namespace leap {
	namespace image {
		inline void init(IMG_InitFlags flag) {
			if (IMG_Init(flag) != flag)
				except::throw_exc();
		}
		inline pointer::SurfacePtr load(const std::string &path) {
			return pointer::make_surface(IMG_Load(path.c_str()));
		}
		inline pointer::TexturePtr convert(const render::Renderer &renderer, const surface::Surface &surface) {
			return pointer::make_texture(renderer.convert(surface));
		}
		inline pointer::TexturePtr convert(const render::Renderer &renderer, const pointer::SurfacePtr &surface) {
			return pointer::make_texture(renderer.convert(*surface));
		}
	}
}

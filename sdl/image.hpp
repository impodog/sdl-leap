#pragma once

#include "const.h"
#include "pointer.hpp"

namespace leap {
	namespace image {
		void init(IMG_InitFlags flag) {
			if (IMG_Init(flag) != flag)
				except::throw_exc();
		}
		pointer::SurfacePtr load(const std::string &path) {
			return pointer::make_surface(IMG_Load(path.c_str()));
		}
		pointer::TexturePtr convert(const render::Renderer &renderer, const surface::Surface &surface) {
			return pointer::make_texture(renderer.convert(surface));
		}
		pointer::TexturePtr convert(const render::Renderer &renderer, const pointer::SurfacePtr &surface) {
			return pointer::make_texture(renderer.convert(*surface));
		}
	}
}

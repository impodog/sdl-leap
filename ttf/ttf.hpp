#pragma once

#include "../sdl/sdl_packs.h"
#include "SDL_ttf.h"
#include <utility>
#include <unordered_map>
#include <string>

namespace leap {
	namespace ttf {
		class Font {
			TTF_Font *font_;

			static pointer::SurfacePtr check(SDL_Surface *surface) {
				if (surface == nullptr)
					except::throw_exc();
				return pointer::make_surface(surface);
			}

		public:
			Font(const char *path, int size) {
				font_ = TTF_OpenFont(path, size);
				if (font_ == nullptr)
					except::throw_exc();
			}

			~Font() noexcept {
				TTF_CloseFont(font_);
			}

			TTF_Font *get() const noexcept {
				return font_;
			}

			pointer::SurfacePtr render_solid(const std::string &text, const SDL_Color &color) const {
				return check(TTF_RenderUTF8_Solid(font_, text.c_str(), color));
			}

			pointer::SurfacePtr render_solid_wrapped(const std::string &text, const SDL_Color &color,
			                                         int wrap = -1) const {
				return check(TTF_RenderUTF8_Solid_Wrapped(font_, text.c_str(), color, wrap));
			}

			pointer::SurfacePtr render_blended(const std::string &text, const SDL_Color &color) const {
				return check(TTF_RenderUTF8_Blended(font_, text.c_str(), color));
			}

			pointer::SurfacePtr render_blended_wrapped(const std::string &text, const SDL_Color &color,
			                                           int wrap = -1) const {
				return check(TTF_RenderUTF8_Blended_Wrapped(font_, text.c_str(), color, wrap));
			}

			const char *family_name() const noexcept {
				return TTF_FontFaceFamilyName(font_);
			}
		};

		using FontPtr = std::shared_ptr<Font>;

		class FontFamily {
			std::string path_;
			std::unordered_map<int, FontPtr> fonts_;

			FontPtr add(int size) {
				auto ptr = std::make_shared<Font>(path_.c_str(), size);
				fonts_.emplace(size, ptr);
				return ptr;
			}

			const FontPtr &get(int size) const {
				return fonts_.at(size);
			}

		public:
			explicit FontFamily(std::string path) : path_(std::move(path)) {}

			FontPtr at(int size) {
				try {
					return get(size);
				}
				catch (std::out_of_range &) {
					return add(size);
				}
			}

			FontPtr at_unchecked(int size) const {
				return get(size);
			}
		};

		using FontFamilyPtr = std::shared_ptr<FontFamily>;
	}

	namespace pointer {
		using ttf::FontPtr;
		using ttf::FontFamilyPtr;

		template <typename... Types>
		FontPtr make_font(Types &&... args) {
			return std::make_shared<ttf::Font>(std::forward<Types>(args)...);
		}

		template <typename... Types>
		FontPtr make_font_family(Types &&... args) {
			return std::make_shared<ttf::FontFamily>(std::forward<Types>(args)...);
		}
	}
}

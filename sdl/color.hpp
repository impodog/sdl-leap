#pragma once
#include "const.h"
#include <limits>
#include <algorithm>

namespace leap {
	namespace color {
		class Color : public SDL_Color {
		protected:
			using int_type = decltype(SDL_Color().r);
			static constexpr int_type max = std::numeric_limits<int_type>::max();

		public:
			Color(int_type r, int_type g, int_type b) : SDL_Color{r, g, b} {}
			Color(int_type r, int_type g, int_type b, int_type a) : SDL_Color{r, g, b, a} {}

			/**
			 * \brief merge two rgb colors by getting average of each color
			 * \details this prevents overflowing, but ignores alpha levels
			 * \param other the other color to merge with
			 * \return the new color of the mix of the two colors
			 */
			[[nodiscard]] Color merge_rgb(const Color &other) const noexcept {
				return {
					static_cast<int_type>(((static_cast<int>(r) + other.r) >> 1)),
					static_cast<int_type>(((static_cast<int>(g) + other.g) >> 1)),
					static_cast<int_type>(((static_cast<int>(b) + other.b) >> 1)),
				};
			}

			/**
			 * \brief merge two rgba colors by getting average of each color
			 * \details this prevents overflowing, and considers alpha levels
			 * \return the new color of the mix of the two colors
			 */
			[[nodiscard]] Color merge(const Color &other) const noexcept {
				const int a1 = max - a, a2 = max - other.a;
				return {
					static_cast<int_type>(((static_cast<int>(r) * a1 + other.r * a2) >> 1) / max),
					static_cast<int_type>(((static_cast<int>(g) * a1 + other.g * a2) >> 1) / max),
					static_cast<int_type>(((static_cast<int>(b) * a1 + other.b * a2) >> 1) / max),
					static_cast<int_type>(((static_cast<int>(a) + other.a) >> 1))
				};
			}

			/**
			 * \brief change the brightness by \c value, positive for brighter and negative for darker
			 * \param value the value of brightness change (0 ~ 255)
			 * \return a new color with the brightness changed
			 */
			[[nodiscard]] Color dim(int value) const noexcept {
				return {
					static_cast<int_type>(std::clamp(static_cast<int>(r) + value, 0, static_cast<int>(max))),
					static_cast<int_type>(std::clamp(static_cast<int>(g) + value, 0, static_cast<int>(max))),
					static_cast<int_type>(std::clamp(static_cast<int>(b) + value, 0, static_cast<int>(max))),
					a
				};
			}
		};
	}
}

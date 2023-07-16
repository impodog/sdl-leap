#pragma once

#include "position.hpp"
#include <string>

namespace leap {
	namespace util {
		template<typename Arithmetic>
		std::string to_string(const leap::pos::Point<Arithmetic> &point) {
			return "(" + std::to_string(point.x) + ", " + std::to_string(point.y) + ")";
		}

		template<typename Arithmetic>
		std::string to_string(const leap::pos::Rect<Arithmetic>& rect) {
			return "(" + std::to_string(rect.x) + ", " + std::to_string(rect.y) + ", " + std::to_string(rect.w) + ", " + std::to_string(rect.h) + ")";
		}
	}
}
#pragma once
#include "const.h"
#include <stdexcept>

namespace leap {
	namespace except {
		class LeapException : public std::runtime_error {
		public:
			LeapException(const char *message) : std::runtime_error(message) {}

			LeapException(const std::string &message) : std::runtime_error(message.c_str()) {}
		};

		inline void throw_exc() {
			throw LeapException(SDL_GetError());
		}
	}
}

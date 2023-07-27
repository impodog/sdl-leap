#pragma once
#include "const.h"
#include <stdexcept>
#include <functional>

namespace leap::except {
	class LeapException : public std::runtime_error {
	public:
		LeapException(const char *message) : std::runtime_error(message) {}

		LeapException(const std::string &message) : std::runtime_error(message.c_str()) {}
	};

	[[noreturn]] inline void throw_exc() {
		throw LeapException(SDL_GetError());
	}

	using ExceptMsg = std::function<const char*()>;

	[[noreturn]] inline void throw_exc(const ExceptMsg &msg) {
		throw LeapException(msg());
	}
}

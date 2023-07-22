#pragma once

#include "const.h"

namespace leap {
	namespace input {
		namespace mouse {
			using MouseButtonType = decltype(SDL_MouseButtonEvent().button);
			constexpr MouseButtonType left = 1, middle = 2, right = 3, wheel_up = 4, wheel_down = 5;

			class Mouse {
				SDL_MouseButtonEvent button_;
				SDL_MouseMotionEvent motion_;
				SDL_MouseWheelEvent wheel_;

			public:
				Mouse() = default;

				~Mouse() = default;

				void button(const SDL_MouseButtonEvent &button) noexcept {
					button_ = button;
				}

				void motion(const SDL_MouseMotionEvent &motion) noexcept {
					motion_ = motion;
				}

				void wheel(const SDL_MouseWheelEvent &wheel) noexcept {
					wheel_ = wheel;
				}

				bool pressed(MouseButtonType button) const {
					switch (button) {
					case 1:
						if (button_.type == SDL_MOUSEBUTTONDOWN)
							return button_.button == SDL_BUTTON_LEFT;
						break;
					case 2:
						if (button_.type == SDL_MOUSEBUTTONDOWN)
							return button_.button == SDL_BUTTON_MIDDLE;
						break;
					case 3:
						if (button_.type == SDL_MOUSEBUTTONDOWN)
							return button_.button == SDL_BUTTON_RIGHT;
						break;
					case 4:
						return wheel_.y > 0;
					case 5:
						return wheel_.y < 0;
					default:
						throw except::LeapException("Invalid mouse button " + std::to_string(button));
					}
					return false;
				}

				pos::IPoint get_position() const noexcept {
					return {motion_.x, motion_.y};
				}

				pos::IPoint get_motion() const noexcept {
					return {motion_.xrel, motion_.yrel};
				}

				pos::IPoint get_wheel() const noexcept {
					return {wheel_.x, wheel_.y};
				}
			};

			using MousePtr = std::shared_ptr<Mouse>;

			inline MousePtr make_mouse() {
				return std::make_shared<Mouse>();
			}
		}
	}

	namespace pointer {
		using input::mouse::MousePtr;
		using input::mouse::make_mouse;
	}
}

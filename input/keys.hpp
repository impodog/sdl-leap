#pragma once

#include "const.h"
#include <unordered_map>
#include <unordered_set>

namespace leap {
	namespace input {
		namespace keys {
			using Keycode = SDL_Keycode;
			using KeySet = std::unordered_set<Keycode>;

			/**
		 * \brief A simple manager for keyboard input.
		 */
			class KeyMap {
				KeySet key_down_;
				Keycode key_pressed_ = SDLK_UNKNOWN;

			public:
				KeyMap() = default;

				void keyboard(const SDL_KeyboardEvent &event) noexcept {
					if (event.type == SDL_KEYDOWN) {
						key_down(event.keysym.sym);
					}
					else if (event.type == SDL_KEYUP) {
						key_up(event.keysym.sym);
					}
				}

				void key_down(const Keycode &key) noexcept {
					key_pressed_ = key;
					key_down_.insert(key);
				}

				void key_up(const Keycode &key) noexcept {
					key_down_.erase(key);
				}

				bool is_down(const Keycode &key) const noexcept {
					return key_down_.contains(key);
				}

				bool is_up(const Keycode &key) const noexcept {
					return !is_down(key);
				}

				const Keycode &pressed() const noexcept {
					return key_pressed_;
				}
			};

			using KeyMapPtr = std::shared_ptr<KeyMap>;

			inline KeyMapPtr make_key_map() {
				return std::make_shared<KeyMap>();
			}
		}
	}

	namespace pointer {
		using input::keys::KeyMapPtr;
		using input::keys::make_key_map;
	}
}

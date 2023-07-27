#include "const.h"

namespace leap {
	namespace input::text_input {
		class TextInput {
			std::string text_, composition_;
			size_t cursor_, selection_;
		public:
			TextInput() noexcept : cursor_(0), selection_(0) {}

			void start() noexcept {
				SDL_StartTextInput();
			}

			void end() noexcept {
				SDL_StopTextInput();
			}

			void input(const SDL_TextInputEvent &input) noexcept {
				text_.append(input.text);
			}

			void edit(const SDL_TextEditingEvent &edit) noexcept {
				composition_ = edit.text;
				cursor_ = edit.start;
				selection_ = edit.length;
			}

			const std::string &get_text() const noexcept {
				return text_;
			}

			const std::string& get_composition() const noexcept {
				return composition_;
			}

			std::string get_selected() const noexcept {
				return text_.substr(cursor_, selection_); 
			}

			size_t get_cursor() const noexcept {
				return cursor_;
			}

			size_t get_selection() const noexcept {
				return selection_;
			}
		};
	}
}
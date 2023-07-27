#include "const.h"
#include "base.hpp"
#include "button.hpp"
#include <list>


namespace leap {
	namespace widget {
		namespace input_box {
			using CharList = std::list<int>;

			class InputBox : public Widget {
			public:
				struct StatusType : Status {
					bool focused, shift=false;
					CharList::iterator cursor;
					pos::IRect range;
					CharList &chars;

					StatusType(bool focused, const pos::IRect &range, CharList &chars) :
						focused(focused), cursor(chars.end()), range(range), chars(chars) { }
				};

				using focus_changer = std::function<bool(const StatusType &)>;
				using cursor_mover = std::function<void(const StatusType &, CharList::iterator &)>;
				using inputer = std::function<int(const StatusType &, bool &shift)>;
				using back_drawer = std::function<void(const render::Renderer &, const StatusType &)>;
				using text_drawer = std::function<void(const render::Renderer &, const StatusType &)>;

			private:
				CharList chars_;

				focus_changer focus_changer_;
				cursor_mover cursor_mover_;
				inputer inputer_;
				back_drawer back_drawer_;
				text_drawer text_drawer_;

				StatusType status_;

				void feed_input(int input) {
					switch (input) {
					case -1:
					case 0:
						break;
					case '\b':
						if (!chars_.empty()) {
							status_.cursor = chars_.erase(--status_.cursor);
						}
						break;
					default:
						status_.cursor = ++chars_.insert(status_.cursor, input);

						break;
					}
				}

			public:
				InputBox(focus_changer focus_changer, cursor_mover cursor_mover, inputer inputer,
				         back_drawer back_drawer, text_drawer text_drawer, const pos::IRect &range) :
					focus_changer_(std::move(focus_changer)), cursor_mover_(std::move(cursor_mover)),
					inputer_(std::move(inputer)), back_drawer_(std::move(back_drawer)),
					text_drawer_(std::move(text_drawer)),
					status_(false, range, chars_) {}

				void draw(const render::Renderer &renderer) override {
					back_drawer_(renderer, status_);
					text_drawer_(renderer, status_);
				}

				void update() override {
					if (focus_changer_(status_)) {
						status_.focused = !status_.focused;
					}
					if (status_.focused) {
						cursor_mover_(status_, status_.cursor);
						feed_input(inputer_(status_, status_.shift));
					}
				}

				StatusPtr status() const noexcept override {
					return &status_;
				}

				const StatusType &get_status() const noexcept {
					return status_;
				}

				void move_to(const pos::IPoint &pos) {
					status_.range.move_to(pos);
				}
			};

			namespace mouse {
				inline InputBox::focus_changer make_focus_changer(const pointer::MousePtr &mouse) {
					return [mouse](const InputBox::StatusType &status) {
						static bool last = false;
						const bool pressed = mouse->pressed(input::mouse::left);
						const bool cur = !last && pressed;
						last = pressed;
						return cur && status.range.contains(mouse->get_position());
					};
				}
			}

			struct Style {
				pointer::TexturePtr background, foreground;
			};

			using StylePtr = std::shared_ptr<Style>;

			template <typename... Types>
			StylePtr make_style(const pointer::TexturePtr &background, const pointer::TexturePtr &foreground) {
				return std::make_shared<Style>(background, foreground);
			}

			inline InputBox::cursor_mover make_cursor_mover(const pointer::KeyMapPtr &key_map) {
				return [key_map](const InputBox::StatusType &status, CharList::iterator &cursor) {
					static bool left = false, right = false;
					const bool left_pressed = key_map->is_down(SDLK_LEFT),
					           right_pressed = key_map->is_down(SDLK_RIGHT);
					const bool left_cur = !left && left_pressed,
					           right_cur = !right && right_pressed;
					left = left_pressed;
					right = right_pressed;
					if (left_cur && cursor != status.chars.begin()) {
						--cursor;
					}
					else if (right_cur && cursor != status.chars.end()) {
						++cursor;
					}
				};
			}
			
			static bool is_usable(const input::keys::Keycode input) {
				return (isascii(input) && isprint(input));
			}

			static int to_usable(const input::keys::Keycode input) {
				switch (input) {
				case '\r':
				case '\n':
					return '\n';
				case '\b':
					return '\b';
				case SDLK_LSHIFT:
				case SDLK_RSHIFT:
				case SDLK_LEFT:
				case SDLK_RIGHT:
					return -1;
				default:
					return input;
				}
			}

			static int shift_key(const input::keys::Keycode input) {
				if (isalpha(input)) {
					return toupper(input);
				}
				switch (input) {
				case SDLK_1:
					return SDLK_EXCLAIM;
				case SDLK_2:
					return SDLK_AT;
				case SDLK_3:
					return SDLK_HASH;
				case SDLK_4:
					return SDLK_DOLLAR;
				case SDLK_5:
					return SDLK_PERCENT;
				case SDLK_6:
					return SDLK_CARET;
				case SDLK_7:
					return SDLK_AMPERSAND;
				case SDLK_8:
					return SDLK_ASTERISK;
				case SDLK_9:
					return SDLK_LEFTPAREN;
				case SDLK_0:
					return SDLK_RIGHTPAREN;
				case SDLK_MINUS:
					return SDLK_UNDERSCORE;
				case SDLK_EQUALS:
					return SDLK_PLUS;
				case SDLK_LEFTBRACKET:
					return '{';
				case SDLK_RIGHTBRACKET:
					return '}';
				case SDLK_BACKSLASH:
					return '|';
				case SDLK_SEMICOLON:
					return SDLK_COLON;
				case SDLK_QUOTE:
					return SDLK_QUOTEDBL;
				case SDLK_COMMA:
					return SDLK_LESS;
				case SDLK_PERIOD:
					return SDLK_GREATER;
				case SDLK_SLASH:
					return SDLK_QUESTION;
				case SDLK_BACKQUOTE:
					return '~';
				default:
					return input;
				}
			}

			inline InputBox::inputer make_inputer(const pointer::KeyMapPtr &key_map) {
				return [key_map](const InputBox::StatusType &status, bool &shift) -> int {
					static int prev = -1;
					const input::keys::Keycode cur = key_map->pressed();
					const int key = to_usable(cur);
					shift = key_map->is_down(SDLK_LSHIFT) || key_map->is_down(SDLK_RSHIFT);
					if (key != -1 && prev != key) {
						prev = key;
						return shift ? shift_key(key) : key;
					}
					if (is_usable(cur) && !key_map->is_down(key)) {
						prev = -1;
					}
					return -1;
				};
			}

			using button::make_style_bit;

			inline InputBox::back_drawer make_back_drawer(const StylePtr &style) {
				return [style](const render::Renderer &renderer, const InputBox::StatusType &status) {
					if (status.focused) {
						style->foreground->copy_to(renderer, status.range);
					}
					else {
						style->background->copy_to(renderer, status.range);
					}
				};
			}

			inline InputBox::text_drawer make_text_drawer(const pointer::FontPtr &font, const SDL_Color &color) {
				return [font, color](const render::Renderer &renderer, const InputBox::StatusType &status) {
					if (!status.chars.empty()) {
						std::string str{status.chars.begin(), status.chars.end()};
						str.insert(std::distance(status.chars.begin(), status.cursor), "|");
						const auto surface = font->render_blended_wrapped(str, color);
						const auto texture = pointer::make_texture(renderer.convert(*surface));
						texture->copy_to(renderer, status.range.left_up());
					}
				};
			}
		}
	}

	namespace pointer {
		namespace input_box {
			using widget::input_box::StylePtr;
		}
	}
}

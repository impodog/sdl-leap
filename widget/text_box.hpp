#include "const.h"
#include "base.hpp"
#include "../ttf/ttf_packs.h"
#include <string>
#include <list>

namespace leap {
	namespace widget {
		namespace text_box {
			struct Text {
				pointer::FontPtr font;
				color::Color color;
				std::string value;
			};

			using TextPtr = std::shared_ptr<Text>;

			template <typename... Types>
			TextPtr make_text(Types &&... args) {
				return std::make_shared<Text>(std::forward<Types>(args)...);
			}

			class TextBox : public Widget {
			public:
				using TextList = std::list<TextPtr>;
				using TextureList = std::list<std::pair<pos::IPoint, pointer::TexturePtr>>;

				struct StatusType : Status {
					const TextList &text;

					StatusType(const TextList &text) : text(text) { }
				};

				using formatter = std::function<TextPtr(const std::string &)>;
				using convertor = std::function<pointer::TexturePtr(const render::Renderer &, const TextPtr &)>;

			private:
				formatter formatter_;
				convertor convertor_;
				StatusType status_;

				TextList text_;
				TextureList texture_;

			public:
				TextBox(formatter formatter, convertor convertor) :
					formatter_(std::move(formatter)), convertor_(std::move(convertor)),
					status_(text_) {}

				void draw(const render::Renderer &renderer) override {
					for (auto &text : texture_) {
						text.second->copy_to(renderer, text.first);
					}
				}

				void update() override { }

				/**
				 * \brief adds a new text to the text box, and returns the texture instance
				 * \param renderer the renderer used
				 * \param value the string to be added
				 * \param position where the text will be placed
				 * \return a pointer to the texture instance created
				 */
				pointer::TexturePtr push(const render::Renderer &renderer, const std::string &value, const pos::IPoint &position) {
					auto text = formatter_(value);
					auto texture = convertor_(renderer, text);
					text_.push_back(text);
					texture_.push_back(std::make_pair(position, texture));
					return texture;
				}

				/**
				 * \brief modifies one of the texts and textures of the text box
				 * \param renderer the renderer used
				 * \param text_it the iterator of text to be modified
				 * \param texture_it the iterator of texture to be modified
				 * \return a pointer to the texture instance created
				 */
				pointer::TexturePtr modify(const render::Renderer& renderer, const TextList::iterator &text_it, const TextureList::iterator &texture_it) const {
					auto text = formatter_((*text_it)->value);
					auto texture = convertor_(renderer, text);
					*text_it = text;
					*texture_it = std::make_pair((*texture_it).first, texture);
					return texture;
				}

				/**
				 * \brief erases one of the texts and textures of the text box
				 * \param text_it the iterator of text to be erased
				 * \param texture_it the iterator of texture to be erased]
				 * this also changes the iterators to the next element
				 */
				void erase(TextList::const_iterator &text_it, TextureList::const_iterator &texture_it) {
					text_it = text_.erase(text_it);
					texture_it = texture_.erase(texture_it);
				}

				/**
			 * \brief access to the status of the widget
			 * \return the pointer to the status(may be \c nullptr)
			 */
				StatusPtr status() const noexcept override {
					return &status_;
				}
				
				TextList& get_texts() noexcept {
					return text_;
				}

				TextureList& get_textures() noexcept {
					return texture_;
				}

				const StatusType &get_status() const noexcept {
					return status_;
				}

				const TextList &get_texts() const noexcept {
					return text_;
				}

				const TextureList &get_textures() const noexcept {
					return texture_;
				}
			};

			inline TextBox::formatter create_single_formatter(const pointer::FontPtr &font,const color::Color &color) {
				return [font, color](const std::string& str) -> TextPtr {
					return make_text(font, color, str);
				};
			}

			inline TextBox::convertor create_convertor() {
				return [](const render::Renderer &renderer, const TextPtr &text) -> pointer::TexturePtr {
					const auto surface = text->font->render_blended(text->value, text->color);
					return pointer::make_texture(renderer.convert(*surface));
				};
			}
		}
	}
	namespace pointer {
		using widget::text_box::TextPtr;
		using widget::text_box::make_text;
	}
}

#pragma once

#include "const.h"
#include "base.hpp"

namespace leap {
	namespace widget {
		namespace button {
			class Button : public Widget {
			public:
				struct StatusType : Status {
				private:
					bool clicked_sign_ = false;
					friend class Button;

				public:
					bool is_active = false, is_pressed = false, is_clicked = false;
					pos::IRect range;
				};

				using detector = std::function<bool(const StatusType &)>;
				using clicker = std::function<bool(const StatusType &)>;
				using drawer = std::function<void(const render::Renderer &, const StatusType &)>;

			private:
				detector detector_;
				clicker clicker_;
				drawer drawer_;

				StatusType status_;

			public:
				Button(detector detector, clicker clicker, drawer drawer, const pos::IRect &range) noexcept :
					detector_(std::move(detector)), clicker_(std::move(clicker)), drawer_(std::move(drawer)) {
					status_.range = range;
				}

				void update() override {
					status_.is_clicked = false;

					status_.is_active = detector_(status_);

					if (status_.is_active) {
						status_.is_pressed = clicker_(status_);

						if (status_.is_pressed) {
							if (status_.clicked_sign_)
								status_.is_clicked = false;
							else {
								status_.clicked_sign_ = true;
								status_.is_clicked = true;
							}
						}
						else {
							status_.clicked_sign_ = false;
							status_.is_clicked = false;
						}
					}
					else {
						status_.is_pressed = false;
						status_.clicked_sign_ = false;
						status_.is_clicked = false;
					}
						
				}

				void draw(const render::Renderer &renderer) override {
					drawer_(renderer, status_);
				}

				StatusPtr status() const noexcept override {
					return &status_;
				}

				const StatusType &get_status() const noexcept {
					return status_;
				}

				void move_to(const pos::IPoint &position) {
					status_.range.move_to(position);
				}

				void resize(const pos::IRect &range) {
					status_.range = range;
				}

				const pos::IRect &range() const noexcept {
					return status_.range;
				}
			};

			namespace mouse {
				inline Button::detector make_detector(const pointer::MousePtr &mouse) {
					return [mouse](const Button::StatusType &status) -> bool {
						return status.range.contains(mouse->get_position());
					};
				}

				inline Button::clicker make_clicker(const pointer::MousePtr &mouse) {
					return [mouse](const Button::StatusType &status) -> bool {
						return mouse->pressed(input::mouse::left);
					};
				}
			};


			struct Style {
				pointer::TexturePtr back, front, on_press;
			};

			using StylePtr = std::shared_ptr<Style>;

			inline Button::drawer make_drawer(const StylePtr &style) {
				return [style](const render::Renderer &renderer, const Button::StatusType &status) {
					if (status.is_active) {
						if (status.is_pressed)
							style->on_press->copy_to(renderer, status.range);
						else
							style->front->copy_to(renderer, status.range);
					}

					else
						style->back->copy_to(renderer, status.range);
				};
			}

			inline StylePtr make_style(const pointer::TexturePtr &back, const pointer::TexturePtr &front,
			                           const pointer::TexturePtr &on_press) {
				return std::make_shared<Style>(Style{back, front, on_press});
			}

			/**
			 * \brief makes a part of the style texture using the format provided
			 * \param renderer the renderer required to convert surface to texture
			 * \param back_color background color of the button style(the outline)
			 * \param front_color foreground color of the button style
			 * \param image the image to put in the center of the button, the centering will be automatically done
			 * \param size the size of the button
			 * \param outline the size of the outline in pixels, or the size of the button border
			 * \return a new texture created with the arguments
			 */
			inline pointer::TexturePtr make_style_bit(const render::Renderer &renderer,
			                                          const SDL_Color &back_color, const SDL_Color &front_color,
			                                          const pointer::SurfacePtr &image,
			                                          const pos::IPoint &size, const int outline) {
				pos::Rect front_rect(0, 0, size.x, size.y);
				front_rect = front_rect.shrink(outline);

				const surface::Surface surface(size);
				surface.fill(back_color);
				surface.fill(front_color, front_rect);

				if (image.get()) {
					pos::Rect image_rect = image->get_range().centered(front_rect);
					surface.blit(*image, image_rect);
				}

				return pointer::make_texture(renderer.convert(surface));
			}
		}
	}

	namespace pointer {
		namespace button {
			using widget::button::StylePtr;
		}
	}
}

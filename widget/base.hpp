#pragma once

#include "const.h"
#include <memory>

namespace leap {
	namespace widget {
		class Status {
		public:
			Status() = default;

			virtual ~Status() = default;

			virtual bool is_null() const noexcept {
				return false;
			}
		};
		using StatusPtr = const Status*;

		class NullStatus final : public Status {
		public:
			bool is_null() const noexcept override {
				return true;
			}
		};

		class Widget {
		public:
			Widget() = default;

			virtual ~Widget() = default;

			virtual void draw(const render::Renderer &renderer) = 0;

			virtual void update() = 0;

			/**
			 * \brief access to the status of the widget
			 * \return the pointer to the status(may be \c nullptr)
			 */
			virtual StatusPtr status() const noexcept {
				return nullptr;
			}
		};

		using WidgetPtr = std::shared_ptr<Widget>;
	}

	namespace pointer {
		using widget::StatusPtr;
		using widget::WidgetPtr;
	}
}

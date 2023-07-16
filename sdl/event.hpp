#include "const.h"
#include "position.hpp"

namespace leap::event {
	class Event {
		SDL_Event event_;

	public:
		Event() = default;

		int poll() {
			return SDL_PollEvent(&event_);
		}

		void wait() {
			if (SDL_WaitEvent(&event_) == 0)
				except::throw_exc();
		}

		SDL_Event &operator*() noexcept {
			return event_;
		}

		const SDL_Event &operator*() const noexcept {
			return event_;
		}

		SDL_Event *operator->() noexcept {
			return &event_;
		}
	};
}

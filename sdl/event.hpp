#include "const.h"
#include "position.hpp"

namespace leap::event {
	class Event {
		SDL_Event *event_;

	public:
		Event() {
			event_ = new SDL_Event;
		}

		~Event() {
			delete event_;
		}

		Event(SDL_Event *event_) = delete;

		bool operator==(const Event &other) const noexcept = delete;

		void poll(int *pending = nullptr) const noexcept {
			if (pending)
				*pending = SDL_PollEvent(event_);
			else
				SDL_PollEvent(event_);
		}

		void wait() const {
			if (SDL_WaitEvent(event_) == 0)
				except::throw_exc();
		}

		SDL_Event &operator*() noexcept {
			return *event_;
		}

		const SDL_Event &operator*() const noexcept {
			return *event_;
		}

		SDL_Event *operator->() const noexcept {
			return event_;
		}
	};
}

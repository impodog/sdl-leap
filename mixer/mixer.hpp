#pragma once

#include "../sdl/sdl_packs.hpp"
#include "SDL_mixer.h"
#include <memory>

namespace leap {
	namespace mixer {
		void pause(int channel = -1) {
			Mix_Pause(channel);
		}

		void halt(int channel=-1) {
			if (Mix_HaltChannel(-1)) {
				except::throw_exc();
			}
		}

		void fade_out(int channel, int ms) {
			if (Mix_FadeOutChannel(channel)) {
				except::throw_exc();
			}
		}

		void fade_in(int channel, int ms) {
			if (Mix_FadeInChannel(channel, ms)) {
				except::throw_exc();
			}
		}

		void fade_in_timed(int channel, int ms, int ticks) {
			if (Mix_FadeInChannel(channel, ms, ticks)) {
				except::throw_exc();
			}
		}
		class Chunk {
			Mix_Chunk* chunk_;
		public:
			explicit Chunk(const char *file) : chunk_(Mix_LoadWAV(file)) {
				if (chunk_ == nullptr) {
					except::throw_exc();
				}
			}

			~Chunk() {
				Mix_FreeChunk(chunk_);
			}

			void play(int loops=1, int channel=-1) {
				if (Mix_PlayChannel(channel, chunk_, loops)) {
					except::throw_exc();
				}
			}

			void play_timed(int loops=1, int ms=-1, int channel=-1) {
				if (Mix_PlayChannelTimed(channel, chunk_, loops, ms)) {
					except::throw_exc();
				}
			}
		};
		using ChunkPtr = std::shared_ptr<Chunk>;

		template <typename... Types>
		ChunkPtr make_chunk(Types&&... args) {
			return std::make_shared<Chunk>(std::forward<Types>(args)...);
		}

		class Music {
			Mix_Music* music_;
		public:
			explicit Music(const char *file) : music_(Mix_LoadMUS(file)) {
				if (music_ == nullptr) {
					except::throw_exc();
				}
			}

			~Music() {
				Mix_FreeMusic(music_);
			}

			void play(int loops=-1) {
				if (Mix_PlayMusic(music_, loops)) {
					except::throw_exc();
				}
			}

			void halt() {
				if (Mix_HaltMusic()) {
					except::throw_exc();
				}
			}

			void fade_out(int ms) {
				if (Mix_FadeOutMusic(ms)) {
					except::throw_exc();
				}
			}

			void fade_in(int loops, int ms) {
				if (Mix_FadeInMusic(music_, loops, ms)) {
					except::throw_exc();
				}
			}

			void fade_in_pos(int loops, int ms, double pos) {
				if (Mix_FadeInMusicPos(music_, loops,ms, pos)) {
					except::throw_exc();
				}
			}
		};
		using MusicPtr = std::shared_ptr<Music>;
		template <typename... Types>
		MusicPtr make_music(Types&&... args) {
			return std::make_shared<Music>(std::forward<Types>(args)...);
		}
	}
	namespace pointer {
		using mixer::ChunkPtr;
		using mixer::make_chunk;
		using mixer::MusicPtr;
		using mixer::make_music;
	}
}

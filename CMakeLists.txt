cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(SDL_PATH D:/libraries/SDL2-VC/SDL2-2.28.1)
set(SDL_IMAGE_PATH D:/libraries/SDL2-VC/SDL2_image-2.6.3)
set(SDL_TTF_PATH D:/libraries/SDL2-VC/SDL2_ttf-2.20.2)
set(SDL_MIXER_PATH D:/libraries/SDL2-VC/SDL2_mixer-2.6.3)

include_directories(
	${SDL_PATH}/include
	${SDL_IMAGE_PATH}/include
	${SDL_TTF_PATH}/include
	${SDL_MIXER_PATH}/include
)

link_directories(
	${SDL_PATH}/lib/x64
	${SDL_IMAGE_PATH}/lib/x64
	${SDL_TTF_PATH}/lib/x64
	${SDL_MIXER_PATH}/lib/x64
)

set(SDL_SOURCE
	"sdl/const.h"
	"sdl/except.hpp"
	"sdl/position.hpp"
	"sdl/render.hpp"
	"sdl/texture.hpp"
	"sdl/surface.hpp"
	"sdl/event.hpp"
	"sdl/to_string.hpp"
	"sdl/pointer.hpp"
	"sdl/image.hpp"
	"sdl/sdl_packs.h"
)

set(INPUT_SOURCE
	"input/const.h"
	"input/input_packs.h"
)



set(SOURCE
	${SDL_SOURCE}
	${INPUT_SOURCE}
	"sdl-leap.cpp"
	"sdl-leap.h"
)

project(sdl-leap)

add_library(sdl-leap STATIC ${SOURCE})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET sdl-leap PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(sdl-leap SDL2 SDL2main SDL2_image SDL2_ttf SDL2_mixer)


project(sdl-leap-test)

add_executable(sdl-leap-test test.cpp ${SOURCE})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET sdl-leap-test PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(sdl-leap-test SDL2 SDL2main SDL2_image SDL2_ttf SDL2_mixer)

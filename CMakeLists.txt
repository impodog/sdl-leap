cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_CXX_STANDARD 17)

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
	"sdl/color.hpp"
	"sdl/position.hpp"
	"sdl/render.hpp"
	"sdl/texture.hpp"
	"sdl/surface.hpp"
	"sdl/event.hpp"
	"sdl/to_string.hpp"
	"sdl/pointer.hpp"
	"sdl/sdl_packs.h"
)

set(IMAGE_SOURCE
	"image/image.hpp"
	"image/image_packs.h"
)

set(TTF_SOURCE
	"ttf/ttf.hpp"
	"ttf/ttf_packs.h"
)

set(INPUT_SOURCE
	"input/const.h"
	"input/keys.hpp"
	"input/mouse.hpp"
	"input/text_input.hpp"
	"input/input_packs.h"
)

set(MIXER_SOURCE
	"mixer/mixer.hpp"
	"mixer/mixer_packs.hpp"
)

set(WIDGET_SOURCE
	"widget/const.h"
	"widget/base.hpp"
	"widget/button.hpp"
	"widget/text_box.hpp"
	"widget/input_box.hpp"
	"widget/widget_packs.h"
)


set(SOURCE
	${SDL_SOURCE}
	${IMAGE_SOURCE}
	${TTF_SOURCE}
	${INPUT_SOURCE}
	${MIXER_SOURCE}
	${WIDGET_SOURCE}
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

cmake_minimum_required(VERSION 3.20)

## Configure the dependencies that has to be installed
## Note: this has to be done before the first call to project()
##
if(BUILD_TESTING)
  list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif()

project(orpg
        VERSION 0.0.1
        DESCRIPTION "Multiplayer RPG")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(GNUInstallDirs)
include(FetchContent)
include(CMakePrintHelpers)
include(CompilerWarnings)

set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_CXX_STANDARD_REQUIRED On)
# set(CMAKE_CXX_EXTENSIONS Off)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}) # ensure that the directory exists

add_executable(orpg)
enable_warnings(orpg)

target_sources(orpg PRIVATE
        src/main.cpp
        src/configuration.h
        src/data.h
        src/entities.h
        src/point.h
        src/rect.h
        src/tags.h
        src/tile_map.h
        src/components/collision.h
        src/components/drawable.h
        src/components/layers.h
        src/components/movement_speed.h
        src/components/npc.h
        src/components/player.h
        src/components/position.h
        src/components/triggerable.h
        src/components/velocity.h
        src/entities/blocked.h
        src/entities/chest.h
        src/entities/none.h
        src/entities/portal.h
)

target_include_directories(orpg
        PRIVATE src
        PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# Default is: Desktop build
target_compile_definitions(orpg PRIVATE ORPG_DESKTOP_BUILD)

get_target_property(ORPG_BINARY_NAME orpg NAME)
configure_file(src/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

if (EMSCRIPTEN)
  message(STATUS "Emscripten build")

  target_compile_definitions(orpg PRIVATE ORPG_WEB_BUILD)

  set(ORPG_MODULE_NAME OrpgModule)

  # -----------------------------------------------------------------------------
  # WebAssembly build options
  # -----------------------------------------------------------------------------
  set(emscripten_link_options)

  list(APPEND emscripten_link_options "SHELL:-s MODULARIZE=1")
  list(APPEND emscripten_link_options "SHELL:-s EXPORT_NAME=${ORPG_MODULE_NAME}")
  list(APPEND emscripten_link_options "SHELL:-s SINGLE_FILE=1")
  list(APPEND emscripten_link_options "SHELL:-s ENVIRONMENT=web")
  list(APPEND emscripten_link_options "SHELL:-s ASSERTIONS=1")
  list(APPEND emscripten_link_options "SHELL:-s USE_GLFW=3")
  list(APPEND emscripten_link_options "SHELL:-s ALLOW_MEMORY_GROWTH=1")
  list(APPEND emscripten_link_options "SHELL:-s EXPORTED_FUNCTIONS=_main,_SetWindowSize")
  list(APPEND emscripten_link_options "SHELL:-s EXPORTED_RUNTIME_METHODS=ccall,cwrap")

  target_compile_options(orpg PUBLIC "-O3")

  target_link_options(orpg PUBLIC ${emscripten_link_options} "-O3")

  configure_file(src/orpg.html.in ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/orpg.html @ONLY)

endif ()

find_package(raylib CONFIG REQUIRED)
find_package(EnTT 3.10 REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

target_link_libraries(orpg PRIVATE raylib)
target_link_libraries(orpg PRIVATE EnTT::EnTT)
target_link_libraries(orpg PRIVATE glfw)

include(CTest)
if (BUILD_TESTING) # option is created by CTest
  enable_testing()
  add_subdirectory(tests)
endif ()

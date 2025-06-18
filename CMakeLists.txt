cmake_minimum_required(VERSION 3.20)
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
        PRIVATE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Default is: Desktop build
target_compile_definitions(orpg PRIVATE ORPG_DESKTOP_BUILD)

get_target_property(BINARY_NAME orpg NAME)
configure_file(src/config.h.in ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.h @ONLY)

if (EMSCRIPTEN)
  message(STATUS "Emscripten build")

  target_compile_definitions(orpg PRIVATE ORPG_WEB_BUILD)

  set(ORPG_MODULE_NAME OrpgModule)

  # -----------------------------------------------------------------------------
  # WebAssembly build options
  # -----------------------------------------------------------------------------
  set(emscripten_link_options)
  set(emscripten_compile_options)

  # list(APPEND emscripten_link_options "-lembind")
  # list(APPEND emscripten_link_options "SHELL:-s MODULARIZE=1")
  # list(APPEND emscripten_link_options "SHELL:-s EXPORT_ES6=1")
  # list(APPEND emscripten_link_options "SHELL:-s SINGLE_FILE=1")
  list(APPEND emscripten_link_options "SHELL:-s EXPORT_NAME=${ORPG_MODULE_NAME}")
  list(APPEND emscripten_link_options "SHELL:-s ENVIRONMENT=web")
  list(APPEND emscripten_link_options "SHELL:-s ASSERTIONS=1")
  list(APPEND emscripten_link_options "SHELL:-s USE_GLFW=3")
  # list(APPEND emscripten_link_options "--pre-js Module.js")

  # set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")

  set(emscripten_debug_options)
  set(DEBUGINFO "NONE" CACHE STRING "Type of debug info")
  set_property(CACHE DEBUGINFO PROPERTY
    STRINGS
      NONE              # -g0
      READABLE_JS       # -g1
      PROFILE           # -g2
      DEBUG_NATIVE      # -g3
  )

  if(DEBUGINFO STREQUAL "NONE")
    list(APPEND emscripten_debug_options "-g0")
  elseif(DEBUGINFO STREQUAL "READABLE_JS")
    list(APPEND emscripten_debug_options "-g1")
    list(APPEND emscripten_link_options "SHELL:-s DEMANGLE_SUPPORT=1")
  elseif(DEBUGINFO STREQUAL "PROFILE")
    list(APPEND emscripten_debug_options "-g2")
    list(APPEND emscripten_link_options "SHELL:-s DEMANGLE_SUPPORT=1")
  elseif(DEBUGINFO STREQUAL "DEBUG_NATIVE")
    list(APPEND emscripten_debug_options "-g3")
    list(APPEND emscripten_link_options
      "SHELL:-s ASSERTIONS=1"
      "SHELL:-s DEMANGLE_SUPPORT=1"
    )
  endif()

  # -----------------------------------------------------------------------------
  # Build options
  # -----------------------------------------------------------------------------
  set(emscripten_optimizations)
  set(OPTIMIZE "SMALLEST_WITH_CLOSURE" CACHE STRING "Emscripten optimization")
  set_property(CACHE OPTIMIZE PROPERTY
    STRINGS
      NO_OPTIMIZATION       # -O0
      LITTLE                # -O1
      MORE                  # -O2
      BEST                  # -O3
      SMALL                 # -Os
      SMALLEST              # -Oz
      SMALLEST_WITH_CLOSURE # -Oz --closure 1
  )

  if(OPTIMIZE STREQUAL "NO_OPTIMIZATION")
    list(APPEND emscripten_optimizations "-O0")
  elseif(OPTIMIZE STREQUAL "LITTLE")
    list(APPEND emscripten_optimizations "-O1")
  elseif(OPTIMIZE STREQUAL "MORE")
    list(APPEND emscripten_optimizations "-O2")
  elseif(OPTIMIZE STREQUAL "BEST")
    list(APPEND emscripten_optimizations "-O3")
  elseif(OPTIMIZE STREQUAL "SMALL")
    list(APPEND emscripten_optimizations "-Os")
  elseif(OPTIMIZE STREQUAL "SMALLEST")
    list(APPEND emscripten_optimizations "-Oz")
  elseif(OPTIMIZE STREQUAL "SMALLEST_WITH_CLOSURE")
    list(APPEND emscripten_optimizations "-Oz")
    list(APPEND emscripten_link_options "--closure 1")
  endif()

  target_compile_options(orpg
    PUBLIC
      ${emscripten_compile_options}
      ${emscripten_optimizations}
      ${emscripten_debug_options}
  )

  target_link_options(orpg
    PUBLIC
      ${emscripten_link_options}
      ${emscripten_optimizations}
      ${emscripten_debug_options}
  )

  configure_file(src/orpg.html.in ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/orpg.html @ONLY)

endif ()

find_package(raylib CONFIG REQUIRED)
find_package(EnTT 3.10 REQUIRED)
find_package(sec21 CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

target_link_libraries(orpg PRIVATE raylib)
target_link_libraries(orpg PRIVATE EnTT::EnTT)
target_link_libraries(orpg PRIVATE sec21::sec21)
target_link_libraries(orpg PRIVATE glfw)

include(CTest)
if (BUILD_TESTING) # option is created by CTest
  enable_testing()
  add_subdirectory(tests)
endif ()

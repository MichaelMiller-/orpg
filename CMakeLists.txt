cmake_minimum_required(VERSION 3.20)
project(orpg
        VERSION 0.0.1
        DESCRIPTION "Multiplayer RPG")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(GNUInstallDirs)
include(FetchContent)
include(CMakePrintHelpers)
include(CompilerWarnings)

set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED On)
# set(CMAKE_CXX_EXTENSIONS Off)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

configure_file(src/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
configure_file(data/preferences.json ${CMAKE_CURRENT_BINARY_DIR}/preferences.json COPYONLY)

add_executable(orpg)
enable_warnings(orpg)

target_sources(orpg PRIVATE  src/main.cpp)

target_include_directories(orpg
        PRIVATE src
        PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

if (EMSCRIPTEN)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")
  set(CMAKE_EXECUTABLE_SUFFIX ".html")

  target_compile_definitions(orpg PRIVATE ORPG_WEB_BUILD)

  FetchContent_Declare(
          raylib
          GIT_REPOSITORY https://github.com/raysan5/raylib
          GIT_PROGRESS TRUE
  )

  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED)
    set(BUILD_TESTING OFF)
    set(PLATFORM Web)
    set(EMSCRIPTEN ON)
    set(EMSCRIPTEN_FORCE_COMPILERS ON)
    set(BUILD_EXAMPLES OFF)
    set(USE_EXTERNAL_GLFW OFF)
    set(USE_AUDIO OFF)
    set(CMAKE_BUILD_TYPE Release)
    FetchContent_Populate(raylib)

    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    list(APPEND CMAKE_MODULE_PATH ${raylib_SOURCE_DIR}/cmake)
    # list(APPEND CMAKE_PREFIX_PATH ${raylib_SOURCE_DIR}/cmake)
  endif()

  # find_package(GLEW CONFIG REQUIRED)
  # target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW)

else()
  target_compile_definitions(orpg PRIVATE ORPG_DESKTOP_BUILD)

  find_package(raylib CONFIG REQUIRED)
endif ()

## raygui
FetchContent_Declare(
        raygui
        GIT_REPOSITORY https://github.com/raysan5/raygui
        GIT_PROGRESS TRUE
)
if (NOT raygui_POPULATED)
  FetchContent_Populate(raygui)
  target_include_directories(orpg PRIVATE ${raygui_SOURCE_DIR}/src)
endif()

find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(EnTT 3.10 REQUIRED)
find_package(raylib CONFIG REQUIRED)
find_package(sec21 CONFIG REQUIRED)

# find_path(RAYGUI_INCLUDE_DIRS "raygui.h")
# target_include_directories(orpg PRIVATE ${RAYGUI_INCLUDE_DIRS})

target_link_libraries(orpg PRIVATE spdlog::spdlog spdlog::spdlog_header_only)
target_link_libraries(orpg PRIVATE nlohmann_json nlohmann_json::nlohmann_json)
target_link_libraries(orpg PRIVATE raylib)
target_link_libraries(orpg PRIVATE EnTT::EnTT)
target_link_libraries(orpg PRIVATE sec21::sec21)


include(CTest)
if (BUILD_TESTING) # option is created by CTest
  enable_testing()
  add_subdirectory(tests)
endif ()

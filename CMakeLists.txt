cmake_minimum_required(VERSION 3.1)
project(orpg
        VERSION 0.0.1
        DESCRIPTION "Multiplayer RPG"
        )


# add_subdirectory(interface2sec21)
# add_subdirectory(interface2raylib)

include(GNUInstallDirs)
include(FetchContent)
include(CMakePrintHelpers)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

configure_file(${PROJECT_SOURCE_DIR}/src/config.h.in ${PROJECT_BINARY_DIR}/config.h)

add_executable(${PROJECT_NAME} src/main.cpp)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_include_directories(${PROJECT_NAME}
        PRIVATE src
        PRIVATE ${PROJECT_BINARY_DIR})

if (EMSCRIPTEN)
  include(ExternalProject)
  ## TODO: as of April 2022 vcpkg has no working raylib:wasm32-emspripten package, therefore use this workaround
  ExternalProject_Add(
          raylib-external
          GIT_REPOSITORY https://github.com/raysan5/raylib
          GIT_PROGRESS TRUE
          CMAKE_ARGS -Wno-dev
          -D CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
          -D CMAKE_C_COMPILER=${CMAKE_C_COMPILER}
          -D PLATFORM=Web
          -D EMSCRIPTEN=ON
          -D EMSCRIPTEN_FORCE_COMPILERS=ON
          -D BUILD_EXAMPLES=OFF
          -D OPENGL_VERSION=OFF
          -D USE_EXTERNAL_GLFW=OFF
          -D USE_AUDIO=OFF
          -D CMAKE_BUILD_TYPE=Release
          -D CMAKE_TOOLCHAIN_FILE:PATH=${VCPKG_CHAINLOAD_TOOLCHAIN_FILE}
          -D CMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}
  )

  configure_file(${PROJECT_SOURCE_DIR}/data/template.html ${PROJECT_BINARY_DIR}/template.html COPYONLY)

  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_C_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY --shell-file template.html")
  # This line is used to set your executable to build with the emscripten html template so taht you can directly open it.
  set(CMAKE_EXECUTABLE_SUFFIX ".html")

  add_dependencies(${PROJECT_NAME} raylib-external)

  target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/lib)
else()
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
  target_include_directories(${PROJECT_NAME} PRIVATE ${raygui_SOURCE_DIR}/src)
endif()

find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog spdlog::spdlog_header_only)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json nlohmann_json::nlohmann_json)
# target_link_libraries(${PROJECT_NAME} PRIVATE interface2raylib)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
# target_link_libraries(${PROJECT_NAME} PRIVATE interface2sec21)
target_link_libraries(${PROJECT_NAME} PRIVATE EnTT::EnTT)

include(CTest)
if (BUILD_TESTING) # option is created by CTest
  enable_testing()
  add_subdirectory(tests)
endif ()

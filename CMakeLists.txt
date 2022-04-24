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
configure_file(${PROJECT_SOURCE_DIR}/data/test.lua ${PROJECT_BINARY_DIR}/test.lua COPYONLY)

add_executable(${PROJECT_NAME} src/main.cpp)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_include_directories(${PROJECT_NAME}
        PRIVATE src
        PRIVATE ${PROJECT_BINARY_DIR})

if (EMSCRIPTEN)
  target_compile_definitions(${PROJECT_NAME} PRIVATE ORPG_WEB_BUILD)

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

    cmake_print_variables(raylib_SOURCE_DIR)
    cmake_print_variables(raylib_BINARY_DIR)

    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    list(APPEND CMAKE_MODULE_PATH ${raylib_SOURCE_DIR}/cmake)
    list(APPEND CMAKE_PREFIX_PATH ${raylib_SOURCE_DIR}/cmake)
  endif()

else()
  target_compile_definitions(${PROJECT_NAME} PRIVATE ORPG_DESKTOP_BUILD)

  find_package(raylib CONFIG REQUIRED)
  find_package(sol2 CONFIG REQUIRED)
  find_package(Lua REQUIRED)

  target_link_libraries(${PROJECT_NAME} PRIVATE sol2::sol2)
  target_link_libraries(${PROJECT_NAME} PRIVATE ${LUA_LIBRARIES})
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
find_package(EnTT 3.10 REQUIRED)

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

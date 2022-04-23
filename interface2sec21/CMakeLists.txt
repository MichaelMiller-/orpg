cmake_minimum_required(VERSION 3.9)
# workaround for sec21
project(interface2sec21
        VERSION 0.0.1)

include(GNUInstallDirs)
include(FetchContent)
include(CMakePrintHelpers)

## header-only library
add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

FetchContent_Declare(
        sec21
        GIT_REPOSITORY https://github.com/MichaelMiller-/sec21
        GIT_PROGRESS TRUE
)
# FetchContent_MakeAvailable(sec21)

FetchContent_GetProperties(sec21)
if (NOT sec21_POPULATED)
    set(BUILD_TESTING OFF)
    FetchContent_Populate(sec21)

    cmake_print_variables(sec21_SOURCE_DIR)
    cmake_print_variables(sec21_BINARY_DIR)

    # add_subdirectory(${sec21_SOURCE_DIR} ${sec21_BINARY_DIR})
    list(APPEND CMAKE_MODULE_PATH ${sec21_SOURCE_DIR}/cmake)
    list(APPEND CMAKE_PREFIX_PATH ${sec21_SOURCE_DIR}/cmake)
endif()

find_package(Boost 1.70 REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(sec21 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} INTERFACE Boost::boost)
target_link_libraries(${PROJECT_NAME} INTERFACE nlohmann_json nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} INTERFACE sec21::sec21)

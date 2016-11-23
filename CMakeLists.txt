cmake_minimum_required(VERSION 2.8)
project(orpg CXX)

# if(CMAKE_CXX_COMPILER MATCHES "/em\\+\\+(-[a-zA-Z0-9.])*")
#  set(CMAKE_CXX_COMPILER_ID "Emscripten")
# endif()

## specifying where to look for .cmake files
##
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/deps/sdl2-cmake-scripts")

# add_custom_target(app DEPENDS ${PROJECT_NAME} copy-data-files)
# add_custom_target(style)
# add_custom_target(copy-data-files ALL COMMAND cmake -E copy_directory ${CMAKE_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}/data)
# add_custom_command(TARGET style COMMAND find ${CMAKE_CURRENT_LIST_DIR}/include ${CMAKE_CURRENT_LIST_DIR}/src ${CMAKE_CURRENT_LIST_DIR}/test -iname "*.hpp" -or -iname "*.cpp" | xargs clang-format -i)

##
## TODO different setting for windows and linux
## 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
## set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
## set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
## set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors")
## set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c99-extensions")

# if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Emscripten")
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --emrun")
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2")
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s BINARYEN=1")
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --preload-file data --use-preload-plugins")
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --shell-file ${CMAKE_SOURCE_DIR}/data/template.html")

  #set(WEB "index.html")
  #add_custom_target(web DEPENDS ${WEB} copy-data-files)
  #add_custom_target(emrun)
  #add_custom_command(TARGET emrun COMMAND emrun --port 8080 index.html)
  #add_executable(${WEB} src/main.cpp)

##else()
#   include(FindPkgConfig)
#   pkg_search_module(SDL2 REQUIRED sdl2)
#   pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
#   pkg_search_module(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)

###
###  TODO use package manager
#
  set(SDL2_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/SDL2-2.0.5/include)
  link_directories(${PROJECT_SOURCE_DIR}/deps/SDL2-2.0.5/lib/x86)
  set(SDL2_LIBRARY SDL2.lib SDL2main.lib)

## endif()

include_directories(inc)
include_directories(deps/di/include)
include_directories(deps/sml/include)
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})

enable_testing()
add_subdirectory(test)

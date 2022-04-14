cmake_minimum_required(VERSION 3.1)
project(orpg
        VERSION 0.0.1
        DESCRIPTION "Multiplayer RPG"
        )

include(CMakePrintHelpers)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


include(ExternalProject)
ExternalProject_Add(
        sec21-external
        GIT_REPOSITORY https://github.com/MichaelMiller-/sec21
        GIT_PROGRESS TRUE
        #        STEP_TARGETS configure
        # DEPENDS orpg
        CMAKE_ARGS -Wno-dev
        -D BUILD_TESTING=OFF
        -D CMAKE_BUILD_TYPE=Release
        -D CMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}
        -D CMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}
)

ExternalProject_Get_property(sec21-external INSTALL_DIR)
message("Source dir of myExtProj = ${INSTALL_DIR}")

cmake_print_variables(INSTALL_DIR)

ExternalProject_Get_property(sec21-external SOURCE_DIR)
message("Source dir of myExtProj = ${SOURCE_DIR}")

# ExternalProject_Add_Step(sec21-external configure
#         COMMAND ${CMAKE_COMMAND} -Wno-dev -D BUILD_TESTING=OFF
#         COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR>
# )
# ExternalProject_Add_StepTargets(sec21-external configure)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_BINARY_DIR}/sec21-install-dir/lib/cmake/sec21)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_BINARY_DIR}/lib/cmake/sec21)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_BINARY_DIR}/lib/cmake/raylib)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_BINARY_DIR}/lib/cmake/raylib)
# list(APPEND CMAKE_PREFIX_PATH /home/michael/dev/sec21/cmake-build-debug/fooinstall/lib/cmake/sec21)
# list(APPEND CMAKE_MODULE_PATH /home/michael/dev/sec21/cmake-build-debug/fooinstall/lib/cmake/sec21)
# list(APPEND CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/cmake)

cmake_print_variables(CMAKE_MODULE_PATH)
cmake_print_variables(CMAKE_PREFIX_PATH)

configure_file(${PROJECT_SOURCE_DIR}/src/config.h.in ${PROJECT_BINARY_DIR}/config.h)

add_executable(${PROJECT_NAME} src/main.cpp)

add_dependencies(${PROJECT_NAME} sec21-external)
# ExternalProject_Add_StepDependencies(${PROJECT_NAME} configure sec21-external)
# ExternalProject_Add_StepDependencies(secretsauce build foobar)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_include_directories(${PROJECT_NAME}
        PRIVATE src/inc
        PRIVATE ${PROJECT_BINARY_DIR}
        PRIVATE ${PROJECT_BINARY_DIR}/include)

if (EMSCRIPTEN)
  ## TODO: as of April 2022 vcpkg has no working raylib:wasm32-emspripten package, therefor use this workaround
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
  target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
else()
  find_package(raylib CONFIG REQUIRED)
  target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
endif ()

find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
# find_package(sec21 CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog spdlog::spdlog_header_only)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json nlohmann_json::nlohmann_json)
# target_link_libraries(${PROJECT_NAME} PRIVATE sec21::sec21)

include(CTest)
if (BUILD_TESTING) # option is created by CTest
  enable_testing()
  add_subdirectory(tests)
endif ()

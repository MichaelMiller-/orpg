find_package(Catch2 CONFIG REQUIRED)

## Speed up the compile-time process
add_library(catch_main STATIC catch_main.cpp)
target_link_libraries(catch_main PUBLIC Catch2::Catch2 Catch2::Catch2WithMain)

function(test filename)
    set(target_name test.${filename})
    add_executable(${target_name})

    target_sources(${target_name} PRIVATE ${filename}.cpp)

    target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../src/)

    target_link_libraries(${target_name} PRIVATE catch_main)

    add_test(
            NAME ${target_name}
            COMMAND $<TARGET_FILE:${target_name}>
            WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endfunction()

## Unit tests via catch in alphabetical order
##
test(player)
test(point)
test(rect)

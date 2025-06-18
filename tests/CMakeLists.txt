find_package(Catch2 CONFIG REQUIRED)

function(test filename)
    set(target_name test.${filename})
    add_executable(${target_name})

    target_sources(${target_name} PRIVATE ${filename}.cpp)

    target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../src/)

    target_link_libraries(${target_name} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)

    add_test(NAME ${target_name} COMMAND $<TARGET_FILE:${target_name}>)
endfunction()

## Unit tests via catch in alphabetical order
##
test(player)
test(point)
test(rect)

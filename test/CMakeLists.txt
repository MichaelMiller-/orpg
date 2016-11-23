include_directories(${CMAKE_CURRENT_LIST_DIR})

function(test test)
    string(REPLACE "/" "_" tmp ${test})
    add_executable(test.${tmp} ${CMAKE_CURRENT_LIST_DIR}/${test}.cpp)
    add_test(test.${tmp} test.${tmp})
endfunction()

include_directories(../)
include_directories(../deps/Catch/include)

test(resource_manager)



function(add_sample name)
    add_library(${name} MODULE ${ARGN})
    target_include_directories(${name} PUBLIC ${RAYTRACE_INCLUDE_DIR})
    set_target_properties(${name}
        PROPERTIES
        PREFIX "")
endfunction()

add_sample(cornell_box cornell_box.cpp)
add_sample(marbles marbles.cpp)

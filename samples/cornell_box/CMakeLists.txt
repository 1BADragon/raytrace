

add_library(cornell_box MODULE cornell_box.cpp)
target_include_directories(cornell_box PUBLIC ${RAYTRACE_INCLUDE_DIR})
set_target_properties(cornell_box
    PROPERTIES
    PREFIX "")

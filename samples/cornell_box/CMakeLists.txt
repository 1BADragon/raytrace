

add_library(cornell_box SHARED cornell_box.cpp)
target_include_directories(cornell_box PUBLIC ${RAYTRACE_INCLUDE_DIR})

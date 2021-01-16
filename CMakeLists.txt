cmake_minimum_required(VERSION 3.16)
project(raytrace)


set(srcs
    src/main.cpp src/vec3.cpp src/common.cpp src/color.cpp
    src/hitrecord.cpp src/hittablelist.cpp src/sphere.cpp
    src/ray.cpp src/camera.cpp src/bvhnode.cpp src/aabb.cpp

    src/solidcolor.cpp src/checkertexture.cpp

    src/materials/metal.cpp src/materials/lambertian.cpp
    src/materials/dielectric.cpp
    )

add_executable(${PROJECT_NAME} ${srcs})
target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -flto)
target_include_directories(${PROJECT_NAME} PUBLIC src)
target_link_options(${PROJECT_NAME} PUBLIC -flto)

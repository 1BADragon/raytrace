cmake_minimum_required(VERSION 3.16)
project(raytrace)

add_subdirectory(deps)

set(srcs
    src/main.cpp src/vec3.cpp src/common.cpp src/color.cpp
    src/hitrecord.cpp src/hittablelist.cpp src/ray.cpp
    src/camera.cpp src/bvhnode.cpp src/aabb.cpp src/stb.cpp
    src/translate.cpp src/rotatey.cpp src/constantmedium.cpp

    src/sphere.cpp src/aarect.cpp src/box.cpp

    src/textures/solidcolor.cpp src/textures/checkertexture.cpp
    src/textures/perlin.cpp src/textures/imagetexture.cpp

    src/materials/metal.cpp src/materials/lambertian.cpp
    src/materials/dielectric.cpp src/materials/diffuselight.cpp
    src/materials/isotropic.cpp
    )

add_executable(${PROJECT_NAME} ${srcs})
target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -flto)
target_include_directories(${PROJECT_NAME} PUBLIC src)
target_link_options(${PROJECT_NAME} PUBLIC -flto)
target_link_libraries(${PROJECT_NAME} stb pthread)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    CXX_STANDARD 17
    C_STANDARD 11)

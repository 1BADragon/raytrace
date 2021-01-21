cmake_minimum_required(VERSION 3.16)
project(raytrace)

find_library(CJSON_PATH cjson REQUIRED)

add_subdirectory(deps)

set(srcs
    src/main.cpp src/vec3.cpp src/common.cpp src/color.cpp
    src/hitrecord.cpp src/ray.cpp src/camera.cpp src/stb.cpp
    src/constantmedium.cpp src/aabb.cpp src/jsonparser.cpp
    src/scene.cpp src/scenebuilder.cpp

    src/textures/solidcolor.cpp src/textures/checkertexture.cpp
    src/textures/perlin.cpp src/textures/imagetexture.cpp

    src/materials/metal.cpp src/materials/lambertian.cpp
    src/materials/dielectric.cpp src/materials/diffuselight.cpp
    src/materials/isotropic.cpp

    src/hittables/box.cpp src/hittables/aarect.cpp
    src/hittables/bvhnode.cpp src/hittables/rotatey.cpp
    src/hittables/sphere.cpp src/hittables/box.cpp
    src/hittables/aarect.cpp src/hittables/hittablelist.cpp
    src/hittables/translate.cpp
    )

add_executable(${PROJECT_NAME} ${srcs})
target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -flto)
target_include_directories(${PROJECT_NAME} PUBLIC src)
target_link_options(${PROJECT_NAME} PUBLIC -flto)
target_link_libraries(${PROJECT_NAME} stb ${CJSON_PATH} pthread)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    CXX_STANDARD 17
    C_STANDARD 11)

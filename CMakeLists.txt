cmake_minimum_required(VERSION 3.16)
project(raytrace)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(DEPS_INSTALL_DIR ${CMAKE_BINARY_DIR}/deps_installs)
file(MAKE_DIRECTORY ${DEPS_INSTALL_DIR})

add_subdirectory(deps)

set(RAYTRACE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src)

set(srcs
    src/main.cpp src/vec3.cpp src/common.cpp src/color.cpp
    src/hitrecord.cpp src/ray.cpp src/camera.cpp src/stb.cpp
    src/aabb.cpp src/jsonparser.cpp src/scene.cpp
    src/scenebuilder.cpp src/builderattr.cpp src/onb.cpp
    src/cosinepdf.cpp src/hittablepdf.cpp src/mat3.cpp
    src/image.cpp src/randompdf.cpp

    src/textures/solidcolor.cpp src/textures/checkertexture.cpp
    src/textures/perlin.cpp src/textures/imagetexture.cpp

    src/materials/metal.cpp src/materials/lambertian.cpp
    src/materials/dielectric.cpp src/materials/diffuselight.cpp
    src/materials/isotropic.cpp

    src/hittables/box.cpp src/hittables/aarect.cpp
    src/hittables/bvhnode.cpp src/hittables/rotate.cpp
    src/hittables/sphere.cpp src/hittables/box.cpp
    src/hittables/aarect.cpp src/hittables/hittablelist.cpp
    src/hittables/translate.cpp src/hittables/constantmedium.cpp
    src/hittables/flipface.cpp

    src/mainwindow.cpp src/scenewidget.cpp src/renderengine.cpp
    )

add_executable(${PROJECT_NAME} ${srcs})
target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -flto -march=native
    -rdynamic -ffast-math ${GTK3_CFLAGS_OTHER})
target_include_directories(${PROJECT_NAME} PUBLIC src ${GTK3_INCLUDE_DIRS})
target_link_options(${PROJECT_NAME} PUBLIC -rdynamic -flto -march=native)
target_link_libraries(${PROJECT_NAME} stb cjson  Qt5::Widgets pthread dl)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    CXX_STANDARD 17
    C_STANDARD 11
)


add_subdirectory(samples)

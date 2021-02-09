include(ExternalProject)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE stb/)
target_compile_definitions(stb INTERFACE)

file(MAKE_DIRECTORY ${DEPS_INSTALL_DIR}/include)

ExternalProject_Add(cjson-ext
    GIT_REPOSITORY https://github.com/DaveGamble/cJSON.git
    GIT_TAG v1.7.14
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DENABLE_CJSON_TEST=OFF
        -DENABLE_CJSON_UTILS=OFF -DBUILD_SHARED_LIBS=OFF
    )

add_library(cjson STATIC IMPORTED GLOBAL)
target_include_directories(cjson INTERFACE ${DEPS_INSTALL_DIR}/include)
set_target_properties(cjson
    PROPERTIES
    IMPORTED_LOCATION ${DEPS_INSTALL_DIR}/lib/libcjson.a)
add_dependencies(cjson cjson-ext)

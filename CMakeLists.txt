cmake_minimum_required(VERSION 3.1.0)
project(raytracer)

file(GLOB SOURCES *.cpp)

add_executable(raytracer ${SOURCES})
target_compile_features(raytracer PUBLIC cxx_std_17)
target_compile_options(raytracer PUBLIC
    -Wall
    -Wextra
    -Wno-unused-parameter
    -Wredundant-decls
    -Wcast-align
    -Wmissing-declarations
    -Wmissing-include-dirs
    -Winvalid-pch
    -Wformat=2)

install(TARGETS raytracer DESTINATION bin)

cmake_minimum_required(VERSION 3.0.0)
project(raytracer)

file(GLOB SOURCES *.cpp)

add_executable(raytracer ${SOURCES})

install(TARGETS raytracer DESTINATION bin)

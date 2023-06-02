cmake_minimum_required(VERSION 3.0)

project(Light2D VERSION 0.1)
set(CMAKE_BUILD_PARALLEL_LEVEL 10)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_STANDARD 17)

# Creates executable
file(GLOB STB_SOURCES stb/*.c)
file(GLOB STB_HEADERS stb/*.h)
file(GLOB SRC_HEADERS src/*.h)
file(GLOB SRC_SOURCES src/*.cpp)

add_executable(${PROJECT_NAME} main.cpp ${STB_SOURCE} ${STB_HEADERS} ${SRC_SOURCES} ${SRC_HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE "./stb")

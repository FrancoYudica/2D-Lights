cmake_minimum_required(VERSION 3.0)

project(Light2D VERSION 0.1)
set(CMAKE_BUILD_PARALLEL_LEVEL 10)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_STANDARD 17)

# Creates executable
file(GLOB STB_SOURCES stb/*.c)
file(GLOB STB_HEADERS stb/*.h)
file(GLOB SRC_HEADERS lights2d/lights2d.h lights2d/src/*.h)
file(GLOB SRC_SOURCES lights2d/src/*.cpp)

add_executable(${PROJECT_NAME} main.cpp ${STB_SOURCE} ${STB_HEADERS} ${SRC_SOURCES} ${SRC_HEADERS})
target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_DIRECTORY_PATH="${CMAKE_HOME_DIRECTORY}")

find_package(TBB REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE TBB::tbb)


target_include_directories(${PROJECT_NAME} PRIVATE "./stb")

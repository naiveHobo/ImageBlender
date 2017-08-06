cmake_minimum_required(VERSION 2.8.3)
project(TerrainGenerator)

find_package(OpenCV REQUIRED)

add_executable(TerrainGenerator src/TerrainGenerator.cpp)
target_link_libraries(TerrainGenerator ${OpenCV_LIBS})

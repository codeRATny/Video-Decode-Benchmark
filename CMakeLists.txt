project(videobench)
cmake_minimum_required(VERSION 3.1.3)
set(TARGET_NAME "videobench")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

file(GLOB SRC_LIST
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg-cpp)

add_executable(${TARGET_NAME} ${SRC_LIST})

target_link_libraries(${TARGET_NAME} ffmpeg-cpp)
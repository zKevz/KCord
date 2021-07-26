cmake_minimum_required(VERSION 3.15)

file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

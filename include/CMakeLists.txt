cmake_minimum_required(VERSION 3.15)

file(GLOB GINCLUDE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
target_sources(${PROJECT_NAME} PRIVATE ${GINCLUDE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE .)

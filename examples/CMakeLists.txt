cmake_minimum_required(VERSION 3.15)
project(KCordExample DESCRIPTION "Example Bot" VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

add_executable(${PROJECT_NAME} SimpleBot.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC KCord ${CMAKE_THREAD_LIBS_INIT})

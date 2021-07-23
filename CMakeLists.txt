cmake_minimum_required(VERSION 3.15)
project(KCord DESCRIPTION "C++ Discord API" VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(CPR_BUILD_TESTS OFF)
set(JSON_BuildTests OFF)

add_subdirectory(src)
add_subdirectory(vendor)
add_subdirectory(include)

target_link_libraries(${PROJECT_NAME} PRIVATE cpr nlohmann_json websocketpp ${CMAKE_THREAD_LIBS_INIT})

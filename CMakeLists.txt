cmake_minimum_required(VERSION 3.15)
project(KCord DESCRIPTION "C++ Discord API" VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(CPR_BUILD_TESTS OFF)
set(JSON_BuildTests OFF)

set(TESTING CACHE INTERNAL ON)

find_package(cpr REQUIRED)
find_package(Boost 1.74 REQUIRED COMPONENTS random system thread regex)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(websocketpp REQUIRED)
find_package(nlohmann_json REQUIRED)

add_subdirectory(src)
add_subdirectory(include)

if (TESTING)
	add_subdirectory(examples)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC cpr nlohmann_json::nlohmann_json OpenSSL::SSL Boost::random websocketpp::websocketpp)
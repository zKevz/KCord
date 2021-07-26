cmake_minimum_required(VERSION 3.15)

find_package(boost REQUIRED COMPONENTS filesystem system thread regex)
find_package(Threads REQUIRED)

include(FetchContent)

FetchContent_Declare(websocketpp GIT_REPOSITORY https://github.com/zaphoyd/websocketpp.git GIT_TAG 0.8.2)
FetchContent_MakeAvailable(websocketpp)

FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/whoshuu/cpr.git GIT_TAG c8d33915dbd88ad6c92b258869b03aba06587ff9) # the commit hash for 1.5.0
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(json GIT_REPOSITORY https://github.com/nlohmann/json.git GIT_TAG v3.7.3)
FetchContent_MakeAvailable(json)

add_subdirectory(deps/websocketpp)
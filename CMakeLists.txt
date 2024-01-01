cmake_minimum_required(VERSION 3.22)

project(telnet-server)

set(WARNING_FLAGS "-Wno-unused-variable -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS} -std=c++17")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_UT_TEST OFF)

find_package(Boost 1.83.0 REQUIRED COMPONENTS system)

add_compile_options(-Wall -Wextra -Wpedantic)

add_subdirectory(test)

include_directories(include)

file(GLOB SRC_FILES "src/*.cpp" 
                    "src/detail/*.cpp" 
                    "src/ParserModules/*.cpp"
                    "src/detail/AsioBase/*.cpp")

add_library(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} Boost::system)

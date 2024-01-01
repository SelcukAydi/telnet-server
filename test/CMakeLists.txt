cmake_minimum_required(VERSION 3.22)

project(telnet-server-test)

find_package(Boost 1.83.0 REQUIRED COMPONENTS system)
find_package(GTest REQUIRED)

add_compile_options(-Wall -Wextra -Wpedantic)

include_directories(include ${CMAKE_SOURCE_DIR}/include)

file(GLOB SRC_FILES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE include ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/include/ParserModules)
target_link_libraries(${PROJECT_NAME} Boost::system ${GTEST_LIBRARIES} gtest_main pthread telnet-server)
cmake_minimum_required(VERSION 3.18)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_STANDARD 20)

project(main LANGUAGES CXX)

add_executable(produce_consumer ./produce_consumer.cpp)
add_executable(factory ./factory.cpp)
add_executable(abstract_factory ./abstract_factory.cpp)
add_executable(builder ./builder.cpp)
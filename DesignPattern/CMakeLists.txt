cmake_minimum_required(VERSION 3.18)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_STANDARD 20)

project(main LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fms-extensions")

add_executable(crtp ./CRTP.cpp)
add_executable(mixin ./Mixin.cpp)
add_executable(oldpolymorphism ./OldPolymorphism.cpp)
add_executable(conceptpolymorphism ./ConceptPolymorphism.cpp)
add_executable(attribute ./attribute.cpp)

add_executable(srp ./SRP.cpp)

add_executable(produce_consumer ./produce_consumer.cpp)
add_executable(factory ./factory.cpp)
add_executable(abstract_factory ./abstract_factory.cpp)
add_executable(builder ./builder.cpp)
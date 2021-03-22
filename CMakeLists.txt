cmake_minimum_required(VERSION 3.0)
project(Algebra LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_library(alg INTERFACE)
target_include_directories(alg INTERFACE include)

add_executable(GFPrimeTest test/GFPrimeTest.cc)
target_link_libraries(GFPrimeTest PUBLIC alg)

add_executable(GF2ExtensionTest test/GF2ExtensionTest.cc)
target_link_libraries(GF2ExtensionTest PUBLIC alg)
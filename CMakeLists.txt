cmake_minimum_required(VERSION 3.14)
project(mystl LANGUAGES CXX)

add_library(mystl INTERFACE)
target_include_directories(mystl INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(mystl INTERFACE cxx_std_17)

enable_testing()
add_executable(test_allocator tests/test_allocator.cpp)
target_link_libraries(test_allocator PRIVATE mystl)
add_test(NAME test_allocator COMMAND test_allocator)

cmake_minimum_required(VERSION 3.13)
project(selene_as_dependency_test)

set(CMAKE_CXX_STANDARD 17)

find_package(selene REQUIRED)

add_executable(selene_as_dependency_test main.cpp)
target_link_libraries(selene_as_dependency_test selene::selene stdc++fs)
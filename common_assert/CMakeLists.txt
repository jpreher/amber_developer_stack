cmake_minimum_required(VERSION 2.8.3)
project(common_assert)

find_package(catkin REQUIRED)

catkin_package(
 INCLUDE_DIRS include
)

include_directories(include)

## Add gtest based cpp test target and link libraries
catkin_add_gtest(${PROJECT_NAME}-test test/test_common_assert.cpp)

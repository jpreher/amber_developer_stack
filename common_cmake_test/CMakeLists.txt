cmake_minimum_required(VERSION 2.8.3)
project(common_cmake_test)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  common_cmake
)

find_package(eigen3 REQUIRED)
find_package(yaml-cpp REQUIRED)

message("")
message("Paths:")
message("${CMAKE_MODULE_PATH}")

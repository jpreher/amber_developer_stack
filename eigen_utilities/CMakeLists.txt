cmake_minimum_required(VERSION 2.8.3)
project(eigen_utilities)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  common_assert
  common_cmake
)

# Find and use the system's Eigen3 library
FIND_PACKAGE (Eigen3 3.3.0)

IF (NOT EIGEN3_FOUND)
  MESSAGE (WARNING "Could not find Eigen3 on your system. Install it.")
ENDIF (NOT EIGEN3_FOUND)


###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
 INCLUDE_DIRS
   include
   ${EIGEN3_INCLUDE_DIR}
  CATKIN_DEPENDS
    common_assert
    common_cmake
  DEPENDS Eigen3
)

#############
## Options ##
#############
add_definitions(-std=c++0x)
option(EIGEN_UTILITIES_TEST_EXTERN "Enable testing of compiling separate translation units / source files with different preprocessor flags" OFF)
option(EIGEN_UTILITIES_NDEBUG "Disable debug information" OFF)

if(EIGEN_UTILITIES_NDEBUG)
  add_definitions(-DEIGEN_UTILITIES_NDEBUG)
endif()

###########
## Build ##
###########

include(common_flags)
common_add_vectorization_flags()

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(include)
# TODO: Check names of system library include directories (eigen3)
include_directories(
  ${catkin_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIR}
)

#############
## Testing ##
#############

set(TEST_SRCS
  test/test_runtime_config.cpp
  test/test_assert_size.cpp
  test/test_compare_utilities.cpp
  test/test_container_utilities.cpp
  test/test_operator_utilities.cpp
  test/test_interp_utilities.cpp
)

if(EIGEN_UTILITIES_TEST_EXTERN)
  add_definitions(-DEIGEN_UTILITIES_TEST_EXTERN)
  list(APPEND TEST_SRCS
    test/test_runtime_config_ndebug.cpp
    test/test_runtime_config_clean_eigen.cpp
  )
endif()

## Add gtest based cpp test target and link libraries
catkin_add_gtest(${PROJECT_NAME}-test ${TEST_SRCS})

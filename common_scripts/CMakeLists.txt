cmake_minimum_required(VERSION 2.8.3)
project(common_scripts)

find_package(catkin REQUIRED COMPONENTS)
catkin_python_setup()

catkin_add_env_hooks(${PROJECT_NAME}-setup SHELLS bash DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/env-hooks)

catkin_package()

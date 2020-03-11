# @brief Expose these CMake modules to the math
# @author Eric Cousineau <eacousineau@gmail.com>

# @todo Handle for installing? Use environment variables and environment hooks?
# http://www.cmake.org/Wiki/CMake_Useful_Variables
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_LIST_DIR})

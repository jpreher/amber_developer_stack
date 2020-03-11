# @brief Add flags to enable vectorization for Eigen
# @author Eric Cousineau <eacousineau@gmail.com>
# @author Dr. John D. Yamakoski
# @author Brian Wightman

option(DISABLE_VECTORIZATION "Disable vectorization for Eigen (necesssary for running valgrind benchmarks)" ON)

macro(common_add_vectorization_flags)
  if(NOT DISABLE_VECTORIZATION)
    message("common: Adding vectorization flags")
    find_package(OpenMP REQUIRED)
    add_definitions(${OpenMP_CXX_FLAGS} -march=native -mtune=native)
    # @see http://stackoverflow.com/questions/12399422/how-to-set-linker-flags-for-openmp-in-cmakes-try-compile-function
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  else()
    message("common: Vectorization disabled")
  endif()
endmacro(common_add_vectorization_flags)

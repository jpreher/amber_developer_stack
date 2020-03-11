# README

This package shows how the effects of `common_cmake` should manifest themselves if everything is working correctly. Specifically, it augments the `CMAKE_MODULE_PATH` by the use of `CFG_EXTRAS` option in [catkin_package](http://docs.ros.org/hydro/api/catkin/html/dev_guide/generated_cmake_api.html#catkin-package).

To see the example in action, in your terminal run

	( source $(roscd common_cmake_test)/scripts/quick_test.sh )

Note that the parentheses `()` in bash will make a subshell and isolate the environment variables to that subshell.

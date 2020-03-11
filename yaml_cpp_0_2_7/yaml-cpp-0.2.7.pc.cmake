prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=@CMAKE_INSTALL_PREFIX@
libdir=${prefix}/@LIB_INSTALL_DIR@
includedir=${prefix}/@INCLUDE_INSTALL_ROOT_DIR@

Name: Yaml-cpp
Description: A YAML parser and emitter for C++
Version: @YAML_CPP_VERSION@
Requires:
Libs: -L${libdir} -lyaml-cpp-0.2.7
Cflags: -I${includedir}

#!/bin/bash

pkg="$(cd "$(dirname "$BASH_SOURCE")/.." && pwd)"
cd $pkg

name="$(basename $pkg)"
log_dir="/tmp/$name/log"

bin="./devel/lib/$name/$name-test"

mkdir -p $log_dir

mkcd () 
{ 
    mkdir "$@" && cd "${!#}"
}

test-target-low()
{ (
    set -e -u
    label=$1
    type=$2
    flags="$3"
    dir=build/${label}
    mkcd -p ${dir}
    indent='    '
    log="${log_dir}/${label}.txt"
    {
        cmake ../.. -DCMAKE_BUILD_TYPE=${type} $flags
        make -j4 all && make -j4 tests
    } > "$log" 2>&1 || { echo "[ build error: ${label} ]"; cat $log >&2; false; }
    {
        $bin && pass="true" || pass="false"
    } >> "$log" 2>&1
    echo -e "-   label: ${label}\n    type: ${type}\n    flags: ${flags}\n    pass: $pass\n    log: $log"
) }

test-target()
{
    (
    set -e -u
    type=$1
    test-target-low "type-${type}" "${type}" ""
    test-target-low "type-${type}-extern" "${type}" "-DEIGEN_UTILITIES_TEST_EXTERN=ON"
    test-target-low "type-${type}-ndebug" "${type}" "-DEIGEN_UTILITIES_NDEBUG=ON"
    )
}

set -e -u
echo "results:"
test-target Release
test-target MinSizeRel 
test-target RelWithDebInfo 
test-target Debug 
test-target "" 

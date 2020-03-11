# common_scripts

Very general scripts for use in a catkin workspace.

## Installation

This is meant for develspace for the time being. To install, the easiest way is to just build with `catkin`, which will use `env-hooks` to add the scripts to `$PATH`.

## `catkin_rename_package.sh`

See `catkin_rename_package.sh --help` for more invocation details.

Example:

	catkin_rename_package.sh . original_name silly_name

To see it in action, run `test/test_catkin_pakage_rename.sh` (no installation necessary).

## Todo

*	Make more specific
*	This script currently adds scripts via `env-hooks`. Would be better to use `${DEVELSPACE}/bin`...

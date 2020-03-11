#!/bin/bash

orig=$(cd $(dirname $BASH_SOURCE) && pwd)
dir="$(mktemp -d)"
cd "$dir"

# Tinker with nav2d
git clone https://github.com/skasperski/navigation_2d.git
cd navigation_2d

# Rename the package
$orig/../scripts/catkin_rename_package.sh . nav2d_msgs some_silly_name

# Have git tell use what changed
# Stage the follows so that -C can shows minimal changes
git add -A :/
git diff --cached -C > $orig/nav2d_msgs.patch

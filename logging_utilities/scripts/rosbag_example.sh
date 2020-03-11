#!/bin/bash

label=experiment
stamp=$(date '+%Y-%m-%dT%H-%M%z') # Modified ISO 8061: Same as `date -Im | sed 's#:#-#g'`
prefix="${stamp}_${label}"

bag="${prefix}.bag"
param="${prefix}.rosparam.yaml"

echo "Recording: rosbag, rosparam"

rosbag record -j -O $bag \
	$(rostopic list /proxi) &

rosparam dump $param &

wait

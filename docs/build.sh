#!/bin/bash

source /opt/ros/eloquent/setup.bash
colcon build --cmake-args -DCMAKE_BUILD_TYPE=RELEASE

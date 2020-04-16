# Copyright 2018 Open Source Robotics Foundation, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""Launch tests."""

from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    # param_config = Path(get_package_share_directory('application_node'), 'config', 'test.yaml')
    # print("config_file_path: ", param_config)

    return LaunchDescription([
        Node(
            package='action_node', node_executable='node', output='screen'),
        Node(
            package='service_node', node_executable='node', output='screen'),
        Node(
            package='application_node', node_executable='node', output='screen'),
        # Node(
        #     package='application_node', node_executable='node_test', output='screen', parameters=[param_config]),
    ])

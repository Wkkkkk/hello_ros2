//
// Created by zhihui on 1/14/20.
//

#include <memory>

#include "action_node/action_node.h"

using namespace Action;

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<ActionNode>());

    rclcpp::shutdown();
    return 0;
}
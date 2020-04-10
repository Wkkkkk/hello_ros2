//
// Created by zhihui on 1/14/20.
//

#include <memory>
#include "service_node/service_node.h"

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<ServiceNode>());
    rclcpp::shutdown();

    return 0;
}

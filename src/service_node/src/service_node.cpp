//
// Created by zhihui on 4/9/20.
//
#include <functional>

#include "service_node/service_node.h"

ServiceNode::ServiceNode(const rclcpp::NodeOptions &options)
    : Node("ServiceNode", options) {
    using namespace std::placeholders;

    service_ptr_ = this->create_service<AddTwoFloats>("add_two_floats",
            std::bind(&ServiceNode::add_two_floats_callback, this, _1, _2, _3));
}


//
// Created by zhihui on 4/14/20.
//

#include <std_msgs/msg/string.hpp>

#include "broadcast_handler.h"
#include "schedule_node/data_context_interface.h"

using namespace Schedule;
using namespace Schedule::handler;

void BroadcastHandler::OnRequest(const BroadcastIncomingType &request) {
    const std::string &topic = request.topic();
    if (topic.empty()) {
        Finish(::grpc::Status(::grpc::INTERNAL, "unnamed client"));
    }

    // create a new node for every service call
    std::shared_ptr <rclcpp::Node> node = rclcpp::Node::make_shared("test_pub_node");
    auto publisher = node->create_publisher<std_msgs::msg::String>(topic, 10);
    auto message = std_msgs::msg::String();
    message.data = request.message();
    RCLCPP_INFO(node->get_logger(), "Publishing: '%s'", message.data.c_str());
    publisher->publish(message);

    // send response
    Send(std::make_unique<google::protobuf::Empty>());
}

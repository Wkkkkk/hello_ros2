//
// Created by zhihui on 4/10/20.
//
#include <chrono>

#include "pubsub_node/publisher.h"

using namespace std::chrono_literals;

Publisher::Publisher(const rclcpp::NodeOptions &node_options)
        : Node("Publisher", node_options) {
    publisher_ = this->create_publisher<Message>("contact", 10);

    timer_ = this->create_wall_timer(1s, std::bind(&Publisher::timer_callback, this));
}

void Publisher::timer_callback() {
    auto msg = Message();

    msg.first_name = "John";
    msg.last_name = "Doe";
    msg.age = 30;
    msg.gender = msg.MALE;
    msg.address = "No. " + std::to_string(count_++);

    RCLCPP_INFO(this->get_logger(), "Publish msg %s", msg.address.c_str());
    publisher_->publish(msg);
}

//
// Created by zhihui on 4/10/20.
//

#include "pubsub_node/subscriber.h"

Subscriber::Subscriber(const rclcpp::NodeOptions &node_options)
        : Node("Subscriber", node_options) {
    using namespace std::placeholders;

    subscription_ = this->create_subscription<Message>(
            "contact", 10, std::bind(&Subscriber::topic_callback, this, _1));
}

void Subscriber::topic_callback(const Message::SharedPtr msg) const {
    RCLCPP_INFO(this->get_logger(), "I heard msg %s", msg->address.c_str());
}

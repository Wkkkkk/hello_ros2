//
// Created by zhihui on 4/10/20.
//

#ifndef CPP_PUBSUB_PUBLISHER_H
#define CPP_PUBSUB_PUBLISHER_H

#include <rclcpp/rclcpp.hpp>

#include "tutorials_msgs/msg/contact.hpp"

class Publisher : public rclcpp::Node {
public:
    using Message = tutorials_msgs::msg::Contact;

    Publisher(const rclcpp::NodeOptions &node_options = rclcpp::NodeOptions());

private:
    void timer_callback();

    rclcpp::Publisher<Message>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    size_t count_ = 0;
};

#endif //CPP_PUBSUB_PUBLISHER_H

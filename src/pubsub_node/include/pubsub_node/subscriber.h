//
// Created by zhihui on 4/10/20.
//

#ifndef CPP_PUBSUB_SUBSCRIBER_H
#define CPP_PUBSUB_SUBSCRIBER_H

#include <rclcpp/rclcpp.hpp>

#include "tutorials_msgs/msg/contact.hpp"

class Subscriber : public rclcpp::Node {
public:
    using Message = tutorials_msgs::msg::Contact;

    Subscriber(const rclcpp::NodeOptions &node_options = rclcpp::NodeOptions());

private:
    void topic_callback(const Message::SharedPtr msg) const;

    rclcpp::Subscription<Message>::SharedPtr subscription_;
};


#endif //CPP_PUBSUB_SUBSCRIBER_H

//
// Created by zhihui on 4/10/20.
//

#ifndef SERVICE_NODE_SERVICE_CLIENT_H
#define SERVICE_NODE_SERVICE_CLIENT_H

#include <rclcpp/rclcpp.hpp>

#include "tutorials_msgs/srv/add_two_floats.hpp"

class ServiceClient : public rclcpp::Node{
public:
    using AddTwoFloats = tutorials_msgs::srv::AddTwoFloats;

    explicit ServiceClient(const rclcpp::NodeOptions &options = rclcpp::NodeOptions());

    void send_goal();
private:
    void response_callback(rclcpp::Client<AddTwoFloats>::SharedFuture future);

    rclcpp::Client<AddTwoFloats>::SharedPtr client_ptr_;
    rclcpp::TimerBase::SharedPtr timer_;
};



#endif //SERVICE_NODE_SERVICE_CLIENT_H

//
// Created by zhihui on 4/10/20.
//

#ifndef SERVICE_NODE_SERVICE_CLIENT_H
#define SERVICE_NODE_SERVICE_CLIENT_H

#include <rclcpp/rclcpp.hpp>
#include <tutorials_msgs/srv/sum_numbers.hpp>

#include "tutorials_msgs/srv/add_two_floats.hpp"

class ServiceClient : public rclcpp::Node {
public:
    using ServiceMessage = tutorials_msgs::srv::SumNumbers;
    using ResponseCallback = std::function<void(ServiceMessage::Response::SharedPtr)>;

    explicit ServiceClient(const rclcpp::NodeOptions &options = rclcpp::NodeOptions());

    void set_goal(ServiceMessage::Request::SharedPtr request);

    void set_response_callback(ResponseCallback callback);

    void send_goal();

private:
    void response_callback(rclcpp::Client<ServiceMessage>::SharedFuture future);

    ServiceMessage::Request::SharedPtr request_;
    rclcpp::Client<ServiceMessage>::SharedPtr client_ptr_;
    ResponseCallback response_callback_;
};



#endif //SERVICE_NODE_SERVICE_CLIENT_H

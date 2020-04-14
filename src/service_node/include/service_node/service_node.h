//
// Created by zhihui on 4/9/20.
//

#ifndef SERVICE_NODE_SERVICE_NODE_H
#define SERVICE_NODE_SERVICE_NODE_H

#include <rclcpp/rclcpp.hpp>

#include "tutorials_msgs/srv/sum_numbers.hpp"

class ServiceNode : public rclcpp::Node {
public:
    using ServiceMessage = tutorials_msgs::srv::SumNumbers;

    explicit ServiceNode(const rclcpp::NodeOptions &options = rclcpp::NodeOptions());

private:

    void service_callback(
            const std::shared_ptr <rmw_request_id_t> request_header,
            const std::shared_ptr <ServiceMessage::Request> request,
            const std::shared_ptr <ServiceMessage::Response> response);

    rclcpp::Service<ServiceMessage>::SharedPtr service_ptr_;
};


#endif //SERVICE_NODE_SERVICE_NODE_H

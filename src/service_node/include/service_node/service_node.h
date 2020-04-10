//
// Created by zhihui on 4/9/20.
//

#ifndef SERVICE_NODE_SERVICE_NODE_H
#define SERVICE_NODE_SERVICE_NODE_H

#include <rclcpp/rclcpp.hpp>

#include "tutorials_msgs/srv/add_two_floats.hpp"

class ServiceNode : public rclcpp::Node {
public:
    using AddTwoFloats = tutorials_msgs::srv::AddTwoFloats;

    explicit ServiceNode(const rclcpp::NodeOptions &options = rclcpp::NodeOptions());

private:

    void add_two_floats_callback(
            const std::shared_ptr <rmw_request_id_t> request_header,
            const std::shared_ptr <AddTwoFloats::Request> request,
            const std::shared_ptr <AddTwoFloats::Response> response) {
        (void) request_header;
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"),
                    "request: a: %ld, b: %ld", request->a, request->b);
        response->sum = request->a + request->b;
    }

    rclcpp::Service<AddTwoFloats>::SharedPtr service_ptr_;
};


#endif //SERVICE_NODE_SERVICE_NODE_H

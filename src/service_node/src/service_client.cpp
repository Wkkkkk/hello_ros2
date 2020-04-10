//
// Created by zhihui on 4/10/20.
//

#include <chrono>

#include "service_node/service_client.h"

using namespace std::chrono_literals;

ServiceClient::ServiceClient(const rclcpp::NodeOptions &options)
    : Node("ServiceClient", options){
    client_ptr_ = this->create_client<AddTwoFloats>("add_two_floats");

    timer_ = this->create_wall_timer(1s,std::bind(&ServiceClient::send_goal, this));
}

void ServiceClient::send_goal() {
    using namespace std::placeholders;

    while (!client_ptr_->wait_for_service(1s)) {
        if (!rclcpp::ok()) {
            RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service. Exiting.");
            return;
        }
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service not available, waiting again...");
    }

    auto request = std::make_shared<AddTwoFloats::Request>();
    request->a = 10.0;
    request->b = 11.2;

    auto result = client_ptr_->async_send_request(request,
            std::bind(&ServiceClient::response_callback,this, _1));
}

void ServiceClient::response_callback(rclcpp::Client<AddTwoFloats>::SharedFuture future) {
    RCLCPP_INFO(this->get_logger(), "Sum: %d", future.get()->sum);
}

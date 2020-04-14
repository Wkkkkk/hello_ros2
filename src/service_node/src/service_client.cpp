//
// Created by zhihui on 4/10/20.
//

#include <chrono>

#include "service_node/service_client.h"

using namespace std::chrono_literals;

ServiceClient::ServiceClient(const rclcpp::NodeOptions &options)
        : Node("ServiceClient", options), request_(std::make_shared<ServiceMessage::Request>()) {
    client_ptr_ = this->create_client<ServiceMessage>("sum_numbers");

    // default value
    request_->set__nums({});
}

void ServiceClient::send_goal() {
    using namespace std::placeholders;

    while (!client_ptr_->wait_for_service(1s)) {
        if (!rclcpp::ok()) {
            RCLCPP_ERROR(this->get_logger(), "Interrupted while waiting for the service. Exiting.");
            return;
        }
        RCLCPP_INFO(this->get_logger(), "service not available, waiting again...");
    }

    auto result = client_ptr_->async_send_request(request_,
                                                  std::bind(&ServiceClient::response_callback, this, _1));
}

void ServiceClient::response_callback(rclcpp::Client<ServiceMessage>::SharedFuture future) {
    if (future.valid()) {
        RCLCPP_INFO(this->get_logger(), "Sum: %d", future.get()->sum);

        if (response_callback_) response_callback_(future.get());
    }
}

void ServiceClient::set_goal(ServiceMessage::Request::SharedPtr request) {
    request_ = request;
}

void ServiceClient::set_response_callback(ServiceClient::ResponseCallback callback) {
    response_callback_ = callback;
}

//
// Created by zhihui on 4/9/20.
//

#include <functional>
#include <numeric>

#include "service_node/service_node.h"

ServiceNode::ServiceNode(const rclcpp::NodeOptions &options)
        : Node("ServiceNode", options) {
    using namespace std::placeholders;

    service_ptr_ = this->create_service<ServiceMessage>("sum_numbers",
                                                        std::bind(&ServiceNode::service_callback, this, _1, _2, _3));
}

void ServiceNode::service_callback(const std::shared_ptr <rmw_request_id_t> request_header,
                                   const std::shared_ptr <ServiceMessage::Request> request,
                                   const std::shared_ptr <ServiceMessage::Response> response) {
    (void) request_header;

    const auto &numbers = request->nums;
    for (auto number : numbers) {
        RCLCPP_INFO(this->get_logger(), "incoming num: %ld", number);
    }

    response->sum = std::accumulate(numbers.begin(), numbers.end(), 0l);
}


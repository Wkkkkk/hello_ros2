//
// Created by zhihui on 3/13/20.
//

#include "sum_numbers_handler.h"
#include "schedule_node/data_context_interface.h"

using namespace Schedule;
using namespace Schedule::handler;
using namespace std::chrono_literals;

void SumNumbersHandler::OnRequest(const SumIncomingType &request) {
    if (request.metadata().client_id().empty()) {
        Finish(::grpc::Status(::grpc::INTERNAL, "unnamed client"));
    }

    int64_t order = request.order();
    std::string client_id = request.metadata().client_id();
    std::cout << "Get Request from: " << client_id << std::endl;

    // TODO: add bounding test
    client_id_ = client_id;
    number_list_.push_back(order);
}

void SumNumbersHandler::OnReadsDone() {
    using ServiceMessage = tutorials_msgs::srv::SumNumbers;
    auto response = std::make_unique<SumOutgoingType>();
    response->set_result(0); // default

    // create a new node for every service call
    std::shared_ptr <rclcpp::Node> node = rclcpp::Node::make_shared(client_id_);
    rclcpp::Client<ServiceMessage>::SharedPtr client = node->create_client<ServiceMessage>("sum_numbers");

    auto request = std::make_shared<ServiceMessage::Request>();
    request->set__nums(number_list_);
    std::vector<int64_t>().swap(number_list_); // clear

    while (!client->wait_for_service(1s)) {
        if (!rclcpp::ok()) {
            RCLCPP_ERROR(node->get_logger(), "Interrupted while waiting for the service. Exiting.");
            this->Send(std::move(response));
            return;
        }
        RCLCPP_INFO(node->get_logger(), "service not available, waiting again...");
    }

    // send request
    auto result = client->async_send_request(request);

    // Wait for the result.
    /// WARNING: this needs to return quickly to avoid blocking the handler
    if (rclcpp::spin_until_future_complete(node, result) ==
        rclcpp::executor::FutureReturnCode::SUCCESS) {
        RCLCPP_INFO(node->get_logger(), "Sum: %ld", result.get()->sum);
        response->set_result(result.get()->sum);
    } else {
        RCLCPP_ERROR(node->get_logger(), "Failed to call service add_two_ints");
    }

    RCLCPP_INFO(node->get_logger(), "Send sum: %ld", response->result());
    this->Send(std::move(response));
}

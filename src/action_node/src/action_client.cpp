//
// Created by zhihui on 4/9/20.
//

#include <chrono>

#include "action_node/action_client.h"

using namespace std::chrono_literals;

ActionClient::ActionClient(std::string name, const rclcpp::NodeOptions &node_options)
        : Node(name, node_options), goal_done_(false) {
    this->client_ptr_ = rclcpp_action::create_client<ActionMessage>(
            this->get_node_base_interface(),
            this->get_node_graph_interface(),
            this->get_node_logging_interface(),
            this->get_node_waitables_interface(),
            "fibonacci");

    // default value
    goal_.order = 0;

    // heartbeat test
    timer_ = this->create_wall_timer(1s, [this]() {
        RCLCPP_INFO(this->get_logger(), "node %s is alive", this->get_name());
    });
}

bool ActionClient::is_goal_done() const {
    return this->goal_done_;
}

void ActionClient::cancel_goal() {
    // in case server not available
    if (goal_handle_future_.valid()) {
        auto cancel_result_future = this->client_ptr_->async_cancel_goal(goal_handle_future_.get());
    }
}

void ActionClient::set_goal(ActionMessage::Goal goal) {
    goal_ = goal;
}

void ActionClient::set_processor(ActionClient::FeedbackProcessor processor) {
    feedback_processor_ = processor;
}

void ActionClient::send_goal() {
    using namespace std::placeholders;

    this->goal_done_ = false;

    if (!this->client_ptr_) {
        RCLCPP_ERROR(this->get_logger(), "Action client not initialized");
    }

    if (!this->client_ptr_->wait_for_action_server(std::chrono::seconds(10))) {
        RCLCPP_ERROR(this->get_logger(), "Action server not available after waiting");
        this->goal_done_ = true;
        return;
    }

    auto goal_msg = goal_;

    RCLCPP_INFO(this->get_logger(), "------------------------------------------------");
    RCLCPP_INFO(this->get_logger(), "Sending goal");

    auto send_goal_options = rclcpp_action::Client<ActionMessage>::SendGoalOptions();
    send_goal_options.goal_response_callback =
            std::bind(&ActionClient::goal_response_callback, this, _1);
    send_goal_options.feedback_callback =
            std::bind(&ActionClient::feedback_callback, this, _1, _2);
    send_goal_options.result_callback =
            std::bind(&ActionClient::result_callback, this, _1);
    auto goal_handle_future = this->client_ptr_->async_send_goal(goal_msg, send_goal_options);
}

void ActionClient::goal_response_callback(std::shared_future<ActionGoalHandle::SharedPtr> future) {
    auto goal_handle = future.get();
    if (!goal_handle) {
        RCLCPP_ERROR(this->get_logger(), "Goal was rejected by server");
    } else {
        RCLCPP_INFO(this->get_logger(), "Goal accepted by server, waiting for result");
        goal_handle_future_ = future;
    }
}

void ActionClient::feedback_callback(ActionGoalHandle::SharedPtr,
                                     const std::shared_ptr<const ActionMessage::Feedback> feedback) {
    RCLCPP_INFO(
            this->get_logger(),
            "Next number in sequence received: %"
    PRId64,
            feedback->partial_sequence.back());

    if (feedback_processor_) {
        feedback_processor_(feedback);
    } else {
        std::cout << "no feedback processor" << std::endl;
    }
}

void
ActionClient::result_callback(const rclcpp_action::ClientGoalHandle<ActionClient::ActionMessage>::WrappedResult &result) {
    this->goal_done_ = true;
    RCLCPP_INFO(this->get_logger(), "Goal %d received result", result.goal_id);
    switch (result.code) {
        case rclcpp_action::ResultCode::SUCCEEDED:
            break;
        case rclcpp_action::ResultCode::ABORTED:
            RCLCPP_INFO(this->get_logger(), "Goal was aborted");
            return;
        case rclcpp_action::ResultCode::CANCELED:
            RCLCPP_INFO(this->get_logger(), "Goal was canceled");
            return;
        default:
            RCLCPP_INFO(this->get_logger(), "Unknown result code");
            return;
    }

    for (auto number : result.result->sequence) {
        RCLCPP_INFO(this->get_logger(), "%"
                PRId64, number);
    }
}

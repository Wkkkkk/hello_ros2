//
// Created by zhihui on 4/9/20.
//

#include "action_node/action_node.h"

using namespace Action;

ActionNode::ActionNode(const rclcpp::NodeOptions &options)
        : Node("ActionNode", options) {
    using namespace std::placeholders;

    this->action_server_ = rclcpp_action::create_server<ActionMessage>(
            this->get_node_base_interface(),
            this->get_node_clock_interface(),
            this->get_node_logging_interface(),
            this->get_node_waitables_interface(),
            "fibonacci",
            std::bind(&ActionNode::handle_goal, this, _1, _2),
            std::bind(&ActionNode::handle_cancel, this, _1),
            std::bind(&ActionNode::handle_accepted, this, _1));
}

rclcpp_action::GoalResponse
ActionNode::handle_goal(const rclcpp_action::GoalUUID &uuid, std::shared_ptr<const ActionMessage::Goal> goal) {
    RCLCPP_INFO(this->get_logger(), "Received goal request with order %d", goal->order);
    (void) uuid;
    // Let's reject sequences that are over 9000
    if (goal->order > 9000) {
        return rclcpp_action::GoalResponse::REJECT;
    }
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
}

rclcpp_action::CancelResponse ActionNode::handle_cancel(const std::shared_ptr <ActionServerGoalHandle> goal_handle) {
    RCLCPP_INFO(this->get_logger(), "Received request to cancel goal %d", goal_handle->get_goal_id());
    (void) goal_handle;
    return rclcpp_action::CancelResponse::ACCEPT;
}

void ActionNode::execute(const std::shared_ptr <ActionServerGoalHandle> goal_handle) {
    RCLCPP_INFO(this->get_logger(), "Executing goal %d", goal_handle->get_goal_id());
    rclcpp::Rate loop_rate(1);
    const auto goal = goal_handle->get_goal();
    auto feedback = std::make_shared<ActionMessage::Feedback>();
    auto &sequence = feedback->partial_sequence;
    sequence.push_back(0);
    sequence.push_back(1);
    auto result = std::make_shared<ActionMessage::Result>();

    for (int i = 1; (i < goal->order) && rclcpp::ok(); ++i) {
        // Check if there is a cancel request
        if (goal_handle->is_canceling()) {
            result->sequence = sequence;
            goal_handle->canceled(result);
            RCLCPP_INFO(this->get_logger(), "Goal Canceled");
            return;
        }
        // Update sequence
        sequence.push_back(sequence[i] + sequence[i - 1]);
        // Publish feedback
        goal_handle->publish_feedback(feedback);
        RCLCPP_INFO(this->get_logger(), "Publish Feedback %d", i);

        loop_rate.sleep();
    }

    // Check if goal is done
    if (rclcpp::ok()) {
        result->sequence = sequence;
        goal_handle->succeed(result);
        RCLCPP_INFO(this->get_logger(), "Goal Succeeded");
    }
}

void ActionNode::handle_accepted(const std::shared_ptr <ActionServerGoalHandle> goal_handle) {
    using namespace std::placeholders;
    // this needs to return quickly to avoid blocking the executor, so spin up a new thread
    std::thread{std::bind(&ActionNode::execute, this, _1), goal_handle}.detach();
}

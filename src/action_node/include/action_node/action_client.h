//
// Created by zhihui on 4/9/20.
//

#ifndef ACTION_NODE_ACTION_CLIENT_H
#define ACTION_NODE_ACTION_CLIENT_H

#include <cinttypes>
#include <functional>

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>

#include "action_node/action_node.h"
#include "tutorials_msgs/action/fibonacci.hpp"

class ActionClient : public rclcpp::Node {
public:
    using ActionMessage = tutorials_msgs::action::Fibonacci;
    using ActionGoalHandle = rclcpp_action::ClientGoalHandle<ActionMessage>;
    using ActionFeedback = std::shared_ptr<const ActionMessage::Feedback>;
    using FeedbackProcessor = std::function<void(ActionFeedback)>;

    explicit ActionClient(std::string name, const rclcpp::NodeOptions &node_options = rclcpp::NodeOptions());

    bool is_goal_done() const;

    void set_goal(ActionMessage::Goal goal);

    void set_processor(FeedbackProcessor processor);

    void cancel_goal();

    void send_goal();

private:
    rclcpp_action::Client<ActionMessage>::SharedPtr client_ptr_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::TimerBase::SharedPtr timer2_;
    ActionMessage::Goal goal_;
    FeedbackProcessor feedback_processor_;
    std::shared_future <std::shared_ptr<ActionGoalHandle>> goal_handle_future_;

    bool goal_done_;

    void goal_response_callback(std::shared_future <ActionGoalHandle::SharedPtr> future);

    void feedback_callback(
            ActionGoalHandle::SharedPtr,
            const std::shared_ptr<const ActionMessage::Feedback> feedback);

    void result_callback(const ActionGoalHandle::WrappedResult &result);
};  // class MinimalActionClient


#endif //ACTION_NODE_ACTION_CLIENT_H

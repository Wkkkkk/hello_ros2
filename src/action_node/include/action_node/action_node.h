//
// Created by zhihui on 4/9/20.
//

#ifndef ACTION_NODE_ACTION_NODE_H
#define ACTION_NODE_ACTION_NODE_H

#include <memory>

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>

#include "tutorials_msgs/action/fibonacci.hpp"

namespace Action {

    class ActionNode : public rclcpp::Node {
    public:
        using ActionMessage = tutorials_msgs::action::Fibonacci;
        using ActionServerGoalHandle = rclcpp_action::ServerGoalHandle<ActionMessage>;

        explicit ActionNode(const rclcpp::NodeOptions &options = rclcpp::NodeOptions());

    private:
        rclcpp_action::Server<ActionMessage>::SharedPtr action_server_;

        rclcpp_action::GoalResponse handle_goal(
                const rclcpp_action::GoalUUID &uuid,
                std::shared_ptr<const ActionMessage::Goal> goal);

        rclcpp_action::CancelResponse handle_cancel(
                const std::shared_ptr <ActionServerGoalHandle> goal_handle);

        void execute(const std::shared_ptr <ActionServerGoalHandle> goal_handle);

        void handle_accepted(const std::shared_ptr <ActionServerGoalHandle> goal_handle);

    };  // class MinimalActionServer
}  // namespace Action
#endif //ACTION_NODE_ACTION_NODE_H

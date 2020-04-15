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

namespace Action {

    class ActionClient : public rclcpp::Node {
    public:
        using ActionMessage = tutorials_msgs::action::Fibonacci;
        using ActionClientGoalHandle = rclcpp_action::ClientGoalHandle<ActionMessage>;
        using ActionFeedback = std::shared_ptr<const ActionMessage::Feedback>;
        using FeedbackCallback = std::function<void(ActionFeedback)>;
        using ActionResult = ActionClientGoalHandle::WrappedResult;
        using ResultCallback = std::function<void(const ActionResult &)>;
        using DefaultFunction = std::function<void()>;

        explicit ActionClient(const std::string &name,
                              rclcpp::executors::MultiThreadedExecutor &executor,
                              const rclcpp::NodeOptions &node_options = rclcpp::NodeOptions());

        ~ActionClient() final;

        bool is_goal_done() const;

        void set_goal(ActionMessage::Goal goal);

        void set_feedback_callback(FeedbackCallback callback);

        void set_result_callback(ResultCallback callback);

        void cancel_goal(DefaultFunction callback = nullptr);

        void send_goal();

    private:
        rclcpp_action::Client<ActionMessage>::SharedPtr client_ptr_;
        rclcpp::TimerBase::SharedPtr heartbeat_timer_;
        rclcpp::TimerBase::SharedPtr send_timer_;
        rclcpp::TimerBase::SharedPtr cancel_timer_;
        rclcpp::executors::MultiThreadedExecutor &executor_;

        ActionMessage::Goal goal_;
        FeedbackCallback feedback_callback_;
        ResultCallback result_callback_;
        std::shared_future <std::shared_ptr<ActionClientGoalHandle>> goal_handle_future_;

        bool goal_done_;

        void goal_response_callback(std::shared_future <ActionClientGoalHandle::SharedPtr> future);

        void feedback_callback(
                ActionClientGoalHandle::SharedPtr,
                const ActionFeedback feedback);

        void result_callback(const ActionResult &result);
    };  // class MinimalActionClient
}  // namespace Action

#endif //ACTION_NODE_ACTION_CLIENT_H

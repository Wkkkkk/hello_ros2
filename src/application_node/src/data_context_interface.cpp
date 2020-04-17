//
// Created by zhihui on 12/24/19.
//

#include "application_node/data_server.h"
#include "application_node/data_context_interface.h"

using namespace Application;
using namespace std::chrono_literals;

DataContextInterface::DataContextInterface(ScheduleServer *map_builder_server)
        : map_builder_server_(map_builder_server) {}

void DataContextInterface::remove_action_node(std::shared_ptr <ActionClient> client) {
    std::string client_id = client->get_name();

    // unregister the client
    {
        std::lock_guard lock(mutex_);
        all_action_clients_.erase(client_id);
    }

    auto &executor = map_builder_server_->get_executor();
    // if goal is not done, cancel the goal first and then stop the node
    if (!client->is_goal_done()) {
        auto result = client->cancel_goal();
        if (result) {
            if (executor.spin_until_future_complete(result.value(), 10ms) ==
                rclcpp::executor::FutureReturnCode::SUCCESS) {
                RCLCPP_INFO(client->get_logger(), "Goal canceled");
            } else {
                RCLCPP_ERROR(client->get_logger(), "Failed to cancel goal");
            }
        }
    }

    // stop the node from working immediately
    executor.remove_node(client);
}

std::shared_ptr <ActionClient> DataContextInterface::find_or_create_action_client(const std::string &node_name) {
    using namespace std::placeholders;
    std::shared_ptr <ActionClient> node;

    std::lock_guard lock(mutex_);
    auto iter = all_action_clients_.find(node_name);

    // if there is a valid weak_node_ptr
    if (iter != all_action_clients_.end() && iter->second.lock()) {
        node = iter->second.lock();
    } else {
        auto &executor = map_builder_server_->get_executor();

        // create a new node
        node.reset(new ActionClient(node_name));

        // insert node
        all_action_clients_[node_name] = node;

        // add node to executor and wait for feedback
        /// Node can only be added once
        /// Executor does not maintain a strong reference to nodes(just a weak_ptr, so shared_ptr count won't increase)
        executor.add_node(node);
    }

    return node;
}
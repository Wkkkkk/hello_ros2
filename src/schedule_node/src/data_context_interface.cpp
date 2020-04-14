//
// Created by zhihui on 12/24/19.
//

#include "schedule_node/data_server.h"
#include "schedule_node/data_context_interface.h"

using namespace Schedule;

DataContextInterface::DataContextInterface(ScheduleServer *map_builder_server)
        : map_builder_server_(map_builder_server) {}

void DataContextInterface::add_node(std::shared_ptr <rclcpp::Node> node_ptr) {
    map_builder_server_->exec_.add_node(node_ptr);
}

void DataContextInterface::remove_action_node_by_name(const std::string &node_name) {
    std::lock_guard lock(mutex_);
    all_action_clients_.erase(node_name);
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
        // create a new node
        node.reset(new ActionClient(node_name),
                // call remove_node_by_name() when shared_ptr count is 0
                   std::bind(&DataContextInterface::remove_action_node_by_name, this, node_name));

        // insert node
        all_action_clients_[node_name] = node;

        // add node to executor and wait for feedback
        /// node can only be added once
        this->add_node(node);
    }

    return node;
}

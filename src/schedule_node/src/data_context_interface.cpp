//
// Created by zhihui on 12/24/19.
//

#include "schedule_node/data_server.h"
#include "schedule_node/data_context_interface.h"

using namespace Schedule;

DataContextInterface::DataContextInterface(ScheduleServer *map_builder_server)
        : map_builder_server_(map_builder_server) {}

void DataContextInterface::remove_action_node(std::shared_ptr <ActionClient> client) {
    std::string client_id = client->get_name();

    std::lock_guard lock(mutex_);
    auto iter = all_action_clients_.find(client_id);

    // if goal is not done, cancel the goal first and then stop the node
    if (!client->is_goal_done()) {

        // delay the remove_node() until the goal is canceled
        //TODO: shard_ptr is passed so the client won't be released, to be fixed
        client->cancel_goal([this, client]() {
            this->map_builder_server_->get_executor().remove_node(client);
        });
    } else {
        // stop the node from working immediately
        map_builder_server_->get_executor().remove_node(client);
    }

    // unregister the client
    if (iter != all_action_clients_.end()) {
        all_action_clients_.erase(client_id);
    }
}

void DataContextInterface::remove_action_node_by_name(const std::string &node_name) {
    std::shared_ptr <ActionClient> node;

    std::lock_guard lock(mutex_);
    auto iter = all_action_clients_.find(node_name);

    // if there is a valid weak_node_ptr
    if (iter != all_action_clients_.end() && iter->second.lock()) {
        node = iter->second.lock();

        // stop the node from working
        map_builder_server_->get_executor().remove_node(node);

        all_action_clients_.erase(node_name);
    } else {
        LOG(WARNING) << "trying to remove an unknown node: " << node_name;
    }
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
        node.reset(new ActionClient(node_name, executor));

        // insert node
        all_action_clients_[node_name] = node;

        // add node to executor and wait for feedback
        /// node can only be added once
//        this->add_node_to_executor(node);
        executor.add_node(node);
    }

    return node;
}
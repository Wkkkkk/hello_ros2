//
// Created by zhihui on 12/24/19.
//

#include "client_node/data_server.h"
#include "client_node/data_context_interface.h"

using namespace server;

DataContextInterface::DataContextInterface(DataServer *map_builder_server)
        : map_builder_server_(map_builder_server) {}

void DataContextInterface::add_node(std::shared_ptr <rclcpp::Node> node_ptr) {
    map_builder_server_->exec_.add_node(node_ptr);
}


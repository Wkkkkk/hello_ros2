//
// Created by zhihui on 12/24/19.
//

#ifndef TRAFFICINFODRIVER_DATA_CONTEXT_INTERFACE_H
#define TRAFFICINFODRIVER_DATA_CONTEXT_INTERFACE_H

#include <map>
#include <mutex>
#include <memory>

#include <rclcpp/rclcpp.hpp>
#include <async_grpc/rpc_handler.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/empty.pb.h>

#include "action_node/action_client.h"
#include "service_node/service_client.h"

namespace Schedule {
    using namespace Action;

    const std::string kTestingServerAddress = "localhost:50051";
    const std::size_t kNumThreads = 1;

    class ScheduleServer;

    class DataContextInterface : public async_grpc::ExecutionContext {
    public:
        explicit DataContextInterface(ScheduleServer *map_builder_server);

        std::shared_ptr <ActionClient> find_or_create_action_client(const std::string &node_name);

        void remove_action_node_by_name(const std::string &node_name);

        void remove_action_node(std::shared_ptr <ActionClient> node);

    private:
        ScheduleServer *map_builder_server_;

        std::mutex mutex_; // protect all_action_clients
        std::map <std::string /* node_name */, std::weak_ptr<ActionClient> /* node_ptr */> all_action_clients_;
    };
}


#endif //TRAFFICINFODRIVER_DATA_CONTEXT_INTERFACE_H

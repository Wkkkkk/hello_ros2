//
// Created by zhihui on 12/24/19.
//

#ifndef TRAFFICINFODRIVER_DATA_CONTEXT_INTERFACE_H
#define TRAFFICINFODRIVER_DATA_CONTEXT_INTERFACE_H

#include <memory>

#include <rclcpp/rclcpp.hpp>
#include <async_grpc/rpc_handler.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/empty.pb.h>

namespace server {

    const std::string kTestingServerAddress = "localhost:50051";
    const std::size_t kNumThreads = 3;

    class DataServer;

    class DataContextInterface : public async_grpc::ExecutionContext {
    public:
        explicit DataContextInterface(DataServer *map_builder_server);

        void add_node(std::shared_ptr <rclcpp::Node> node_ptr);

    private:
        DataServer *map_builder_server_;
    };
}


#endif //TRAFFICINFODRIVER_DATA_CONTEXT_INTERFACE_H

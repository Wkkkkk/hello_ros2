//
// Created by zhihui on 12/24/19.
//
#include <async_grpc/completion_queue_pool.h>

#include "client_node/data_context_interface.h"
#include "client_node/data_server.h"

#include "login_handler.h"
//#include "update_position_handler.h"
#include "get_instructions_handler.h"
//#include "bidirectional_test.h"

using namespace server;
using namespace message::handler;

DataServer::DataServer() {
    async_grpc::Server::Builder server_builder;
    server_builder.SetServerAddress(kTestingServerAddress);
    server_builder.SetNumGrpcThreads(kNumThreads);
    server_builder.SetNumEventThreads(kNumThreads);

    server_builder.RegisterHandler<LoginHandler>();
//    server_builder.RegisterHandler<UpdatePositionHandler>();
    server_builder.RegisterHandler<GetInstructionsHandler>();
//    server_builder.RegisterHandler<BidirectionalTest>();
    grpc_server_ = server_builder.Build();

    grpc_server_->SetExecutionContext(std::make_unique<DataContextInterface>(this));
}

DataServer::~DataServer() {
    if (ros_thread_) {
        ros_thread_->join();
    }

    async_grpc::CompletionQueuePool::Shutdown();
}


void DataServer::Start() {
    LOG(INFO) << "Server listening on " << kTestingServerAddress;
    grpc_server_->Start();
}

void DataServer::Shutdown() {
    if (!is_done_) {
        is_done_ = true;
        grpc_server_->Shutdown();
    }
}

void DataServer::WaitForShutdown() {
    grpc_server_->WaitForShutdown();
}

void DataServer::initSlamThread() {
    using namespace std::chrono_literals;

    ros_thread_ = std::make_unique<std::thread>(
            [this]() {
                rclcpp::Rate loop_rate(1);

                // Do work periodically as it becomes available to us.
                // Blocking call, may block indefinitely.
                while (rclcpp::ok()) {
                    exec_.spin_some();

                    loop_rate.sleep();
                }

                // shutdown server
                this->Shutdown();
            });
}
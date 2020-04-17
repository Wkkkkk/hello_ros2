//
// Created by zhihui on 12/24/19.
//
#include <async_grpc/completion_queue_pool.h>

#include "application_node/data_server.h"
#include "application_node/data_context_interface.h"

#include "login_handler.h"
#include "broadcast_handler.h"
#include "sum_numbers_handler.h"
#include "calculate_fibonacci_handler.h"

using namespace Application;
using namespace Application::handler;

ScheduleServer::ScheduleServer() {
    async_grpc::Server::Builder server_builder;
    server_builder.SetServerAddress(kTestingServerAddress);
    server_builder.SetNumGrpcThreads(kNumThreads);
    server_builder.SetNumEventThreads(kNumThreads);

    server_builder.RegisterHandler<LoginHandler>();
    server_builder.RegisterHandler<SumNumbersHandler>();
    server_builder.RegisterHandler<CalculateFibonacciHandler>();
    server_builder.RegisterHandler<BroadcastHandler>();
    grpc_server_ = server_builder.Build();

    grpc_server_->SetExecutionContext(std::make_unique<DataContextInterface>(this));
}

ScheduleServer::~ScheduleServer() {
    if (ros_thread_) {
        ros_thread_->join();
    }

    async_grpc::CompletionQueuePool::Shutdown();
}


void ScheduleServer::start() {
    LOG(INFO) << "Server listening on " << kTestingServerAddress;
    grpc_server_->Start();

    init_ros_thread();
}

void ScheduleServer::shutdown() {
    if (!is_done_) {
        is_done_ = true;
        grpc_server_->Shutdown();
    }
}

void ScheduleServer::wait_for_shutdown() {
    grpc_server_->WaitForShutdown();
}

void ScheduleServer::init_ros_thread() {
    using namespace std::chrono_literals;

    ros_thread_ = std::make_unique<std::thread>(
            [this]() {
                rclcpp::Rate loop_rate(100ms);

                // Do work periodically as it becomes available to us.
                // Blocking call, may block indefinitely.
                while (rclcpp::ok()) {
                    exec_.spin_some();

                    loop_rate.sleep();
                }

                // shutdown server
                this->shutdown();
            });
}

rclcpp::executors::MultiThreadedExecutor &ScheduleServer::get_executor() {
    return exec_;
}

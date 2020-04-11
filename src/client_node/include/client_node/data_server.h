//
// Created by zhihui on 12/24/19.
//

#ifndef TRAFFICINFODRIVER_DATA_SERVER_H
#define TRAFFICINFODRIVER_DATA_SERVER_H

#include <memory>
#include <thread>

#include <rclcpp/rclcpp.hpp>
#include <async_grpc/server.h>

namespace server {
    class DataServer {
    public:
        DataServer();

        ~DataServer();

        void Start();

        void Shutdown();

        void WaitForShutdown();

        void initSlamThread();

        std::unique_ptr <std::thread> ros_thread_;
        rclcpp::executors::MultiThreadedExecutor exec_;
        std::unique_ptr <async_grpc::Server> grpc_server_;
        bool is_done_ = false;
    };
}

#endif //TRAFFICINFODRIVER_DATA_SERVER_H

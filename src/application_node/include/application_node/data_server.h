//
// Created by zhihui on 12/24/19.
//

#ifndef TRAFFICINFODRIVER_DATA_SERVER_H
#define TRAFFICINFODRIVER_DATA_SERVER_H

#include <memory>
#include <thread>

#include <rclcpp/rclcpp.hpp>
#include <async_grpc/server.h>

namespace Application {
    class ScheduleServer {
    public:
        ScheduleServer();

        ~ScheduleServer();

        void start();

        void shutdown();

        void wait_for_shutdown();

        rclcpp::executors::MultiThreadedExecutor &get_executor();

    private:
        void init_ros_thread();

        std::unique_ptr <std::thread> ros_thread_;
        rclcpp::executors::MultiThreadedExecutor exec_;
        std::unique_ptr <async_grpc::Server> grpc_server_;
        bool is_done_ = false;
    };
}

#endif //TRAFFICINFODRIVER_DATA_SERVER_H

//
// Created by zhihui on 4/10/20.
//

#include "client_node/data_server.h"

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    server::DataServer data_server;
    data_server.initSlamThread();
    data_server.Start();

    // block until Shutdown() is called
    data_server.WaitForShutdown();
    rclcpp::shutdown();
    return 0;
}
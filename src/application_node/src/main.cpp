//
// Created by zhihui on 4/10/20.
//

#include "application_node/data_server.h"

using namespace Application;

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    ScheduleServer data_server;
    data_server.start();

    // block until shutdown() is called
    data_server.wait_for_shutdown();
    rclcpp::shutdown();
    return 0;
}
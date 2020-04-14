//
// Created by zhihui on 4/10/20.
//

#include "schedule_node/data_server.h"

using namespace Schedule;

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    ScheduleServer data_server;
    data_server.init_ros_thread();
    data_server.start();

    // block until shutdown() is called
    data_server.wait_for_shutdown();
    rclcpp::shutdown();
    return 0;
}
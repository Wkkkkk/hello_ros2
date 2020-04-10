//
// Created by zhihui on 1/14/20.
//

#include "service_node/service_node.h"
#include "service_node/service_client.h"

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::executors::MultiThreadedExecutor exec;

    // magic!
    rclcpp::NodeOptions options;
    options.use_intra_process_comms(true);

    auto service_server = std::make_shared<ServiceNode>(options);
    auto service_client = std::make_shared<ServiceClient>(options);
    exec.add_node(service_server);
    exec.add_node(service_client);

    exec.spin();
    rclcpp::shutdown();
    return 0;
}
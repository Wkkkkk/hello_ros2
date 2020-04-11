//
// Created by zhihui on 1/14/20.
//

#include <memory>

//#include "action_node/action_node.h"
#include "action_node/action_client.h"

using namespace std::chrono_literals;

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::executors::MultiThreadedExecutor exec;

    // magic!
    rclcpp::NodeOptions options;
    options.use_intra_process_comms(false);

    auto action_server = std::make_shared<ActionNode>(options);
    auto action_client = std::make_shared<ActionClient>("Client1", options);
    exec.add_node(action_server);
    exec.add_node(action_client);

//    auto timer = action_client->create_wall_timer(10s, [action_client]() { action_client->send_goal(); });
    action_client->send_goal();
    exec.spin();
    rclcpp::shutdown();
    return 0;
}

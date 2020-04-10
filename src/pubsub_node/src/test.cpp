//
// Created by zhihui on 1/14/20.
//

#include "pubsub_node/publisher.h"
#include "pubsub_node/subscriber.h"

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::executors::MultiThreadedExecutor exec;

    // magic!
    rclcpp::NodeOptions options;
    options.use_intra_process_comms(true);

    auto publisher = std::make_shared<Publisher>(options);
    auto subscriber = std::make_shared<Subscriber>(options);
    exec.add_node(publisher);
    exec.add_node(subscriber);

    exec.spin();
    rclcpp::shutdown();
    return 0;
}
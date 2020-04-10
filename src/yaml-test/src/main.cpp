//
// Created by zhihui on 4/7/20.
//

#include <rclcpp/rclcpp.hpp>

class TestNode : public rclcpp::Node {
public:
    TestNode()
            : Node("my_test") {
        this->declare_parameter<int>("some_int", 0);
        this->declare_parameter<std::string>("a_string", "");

        do_something();
    }

    void do_something() {
        int some_int;
        this->get_parameter<int>("some_int", some_int);
        RCLCPP_INFO(this->get_logger(), "some_int: %d", some_int);

        std::string a_string;
        this->get_parameter<std::string>("a_string", a_string);
        RCLCPP_INFO(this->get_logger(), "a_string: %s", a_string.c_str());

        // TODO bug fix
        // without this line, do_something() will block until the process is dead
        // not sure whether it is due to get_parameter() or RCLCPP_INFO()
        std::cout << "all is done" << std::endl;
    }

private:
};


int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    auto server = std::make_shared<TestNode>();

    rclcpp::spin(server);

    rclcpp::shutdown();
    return 0;
}
#include "rclcpp/rclcpp.hpp"
#include <iostream>

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("hello_world");
    RCLCPP_INFO(node->get_logger(), "helloworld demo reader");
    rclcpp::shutdown();

    return 0;
}

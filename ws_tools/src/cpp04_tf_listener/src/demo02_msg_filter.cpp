#include "rclcpp/rclcpp.hpp"

class TFPointListener: public rclcpp::Node{
    public:
        TFPointListener():Node("tf_point_listener_node_cpp"){
            RCLCPP_INFO(this->get_logger(), "创建点监听");
        }
};

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TFPointListener>());
    rclcpp::shutdown();
    return 0;
}

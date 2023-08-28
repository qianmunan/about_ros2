#include "rclcpp/rclcpp.hpp"

/**
 * 先发布laser到base_link的坐标相对关系，在发布camera到base_link的坐标系相对关系，
 * 求解laser到camera的坐标相对关系
 */

class TFListener: public rclcpp::Node{
    public:
        TFListener():Node("tf_listener_node_cpp"){
            RCLCPP_INFO(this->get_logger(), "创建了监听方！！！");
        }
};

int main(int argc, char ** argv){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TFListener>());
    rclcpp::shutdown();
    return 0;
}

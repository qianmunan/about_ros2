#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/point_stamped.hpp"

using namespace std::chrono_literals;

class MiniPointNode: public rclcpp::Node{

    public:
        MiniPointNode():Node("mini_point_tf_node_cpp"), x(0.0){
        
            RCLCPP_INFO(this->get_logger(), "hello world");
            // 创建发布方
            point_pub_ = this->create_publisher<geometry_msgs::msg::PointStamped>("point",10);
            // 创建定时器
            timer_ = this->create_wall_timer(1s,std::bind(&MiniPointNode::on_timer,this));
        }
    private:
        // 回调函数
        void on_timer()
        {
            // 组织消息
            geometry_msgs::msg::PointStamped ps;
            ps.header.stamp = this->now();
            ps.header.frame_id = "laser";
            x += 0.05;
            ps.point.x = x;
            ps.point.y = 0.0;
            ps.point.z = -0.1;
            // 发布消息
            point_pub_->publish(ps);
        }
        rclcpp::Publisher<geometry_msgs::msg::PointStamped>::SharedPtr point_pub_;
        rclcpp::TimerBase::SharedPtr timer_;
        double_t x;
};

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);


    rclcpp::spin(std::make_shared<MiniPointNode>());

    rclcpp::shutdown();

    return 0;
}

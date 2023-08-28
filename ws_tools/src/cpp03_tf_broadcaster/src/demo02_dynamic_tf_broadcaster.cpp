#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/transform_broadcaster.h"
#include "turtlesim/msg/pose.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "tf2/LinearMath/Quaternion.h"
class TFDynamicBroadcaster: public rclcpp::Node{

    public:
        TFDynamicBroadcaster():Node("tf_dynamic_broadcaster_node_cpp"){ 
            RCLCPP_INFO(this->get_logger(), "创建动态广播器");

            // 创建一个动态广播器
            broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);

            // 创建一个乌龟位姿订阅方
            pose_sub_ = this->create_subscription<turtlesim::msg::Pose>("turtle1/pose",10,std::bind(&TFDynamicBroadcaster::do_pose, this, std::placeholders::_1));
            // 回调函数中，获取乌龟位姿并生成相对关系然后发布
        }
    private:

        std::shared_ptr<tf2_ros::TransformBroadcaster> broadcaster_;
        rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_sub_;
        // 获取乌龟的位姿
        void do_pose(const turtlesim::msg::Pose & pose)
        {
            // 组织消息，发布
            geometry_msgs::msg::TransformStamped ts;
            ts.header.stamp = this->now();
            ts.header.frame_id = "world";

            ts.child_frame_id = "turtle1";

            ts.transform.translation.x = pose.x;
            ts.transform.translation.y = pose.y;
            ts.transform.translation.z = 0.0;

            // 从欧拉角转换为四元数
            tf2::Quaternion qtn;
            qtn.setRPY(0,0,pose.theta);
            
            ts.transform.rotation.x = qtn.x();
            ts.transform.rotation.y = qtn.y();
            ts.transform.rotation.z = qtn.z();
            ts.transform.rotation.w = qtn.w();



            // 发布
            broadcaster_ -> sendTransform(ts);

        }

};

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<TFDynamicBroadcaster>());
    rclcpp::shutdown();
    return 0;
}

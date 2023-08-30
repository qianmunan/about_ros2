#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/transform_broadcaster.h"
#include "turtlesim/msg/pose.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "tf2/LinearMath/Quaternion.h"
/**
 * 需求：广播不同乌龟相对于world的坐标系相对关系
 * 流程：
 *      1. 包含头文件
 *      2. 初始化ROS2客户端
 *      3. 自定义节点类
 *
 *      4. 调用spain函数，并传入节点对象指针
 *      5. 资源释放
 *
 */

class Exer02TfBroadcaster: public rclcpp::Node{
    public:
        Exer02TfBroadcaster():Node("exer02_tf_broadcaster_node_cpp"){
            RCLCPP_INFO(this->get_logger(), "创建了broadcaster");
            this->declare_parameter("turtle","turtle1");
            turtle = this->get_parameter("turtle").as_string();
            // 创建一个动态广播器
            broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);
            // 创建一个乌龟位资订阅方
            pose_sub_ = this->create_subscription<turtlesim::msg::Pose>("/"+ turtle +"/pose",10,std::bind(&Exer02TfBroadcaster::do_pose,this,std::placeholders::_1));
        }
    private:
        std::string turtle;
        void do_pose(const turtlesim::msg::Pose & pose){
            geometry_msgs::msg::TransformStamped ts;
            ts.header.stamp = this->now();
            ts.header.frame_id = "world";

            ts.child_frame_id = turtle;

            ts.transform.translation.x = pose.x;
            ts.transform.translation.y = pose.y;
            ts.transform.translation.z = 0.0;

            // 从欧拉角转换出四元数
            // 乌龟的欧拉角只有yaw 上的取值，没有roll和pith
            tf2::Quaternion qtn;
            qtn.setRPY(0,0,pose.theta);

            ts.transform.rotation.x = qtn.x();
            ts.transform.rotation.y = qtn.y();
            ts.transform.rotation.z = qtn.z();
            ts.transform.rotation.w = qtn.w();

            // 发布
            broadcaster_->sendTransform(ts);
        }
        std::shared_ptr<tf2_ros::TransformBroadcaster> broadcaster_;
        rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_sub_;
};

int main(int argc, char ** argv){

    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Exer02TfBroadcaster>());
    rclcpp::shutdown();
    return 0;
}

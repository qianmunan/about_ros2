/*
   需求：订阅窗口1中的乌龟速度，然后生成控制窗口2乌龟运动的指令并发布。
   步骤：
       1.包含头文件；
       2.初始化 ROS2 客户端；
       3.定义节点类；
          3-1.创建控制第二个窗体乌龟运动的发布方；
          3-2.创建订阅第一个窗体乌龟pose的订阅方；
          3-3.根据订阅的乌龟的速度生成控制窗口2乌龟运动的速度消息并发布。
       4.调用spin函数，并传入节点对象指针；
       5.释放资源。
*/
// 1.包含头文件；
#include <rclcpp/rclcpp.hpp>
#include <turtlesim/msg/pose.hpp>
#include <geometry_msgs/msg/twist.hpp>
// 3.定义节点类；
class ExePubSub : public rclcpp::Node
{
public:
    ExePubSub() : rclcpp::Node("exer01_pub_sub")
    {
        // 3-1.创建控制第二个窗体乌龟运动的发布方；
        twist_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/t2/turtle1/cmd_vel", 1);
        // 3-2.创建订阅第一个窗体乌龟pose的订阅方；
        pose_sub_ = this->create_subscription<turtlesim::msg::Pose>(
                "/turtle1/pose", 1, std::bind(&ExePubSub::poseCallback, this, std::placeholders::_1));
    }

private:
    // 3-3.根据订阅的乌龟的速度生成控制窗口2乌龟运动的速度消息并发布。
    void poseCallback(const turtlesim::msg::Pose::ConstSharedPtr pose)
    {
        geometry_msgs::msg::Twist twist;
        twist.angular.z = -(pose->angular_velocity); //角速度取反
        twist.linear.x = pose->linear_velocity; //线速度不变
        twist_pub_->publish(twist);
    }

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr twist_pub_;
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_sub_;
};

int main(int argc, char** argv)
{
    // 2.初始化 ROS2 客户端；
    rclcpp::init(argc, argv);
    // 4.调用spin函数，并传入节点对象指针；
    rclcpp::spin(std::make_shared<ExePubSub>());
    // 5.释放资源。
    rclcpp::shutdown();
}
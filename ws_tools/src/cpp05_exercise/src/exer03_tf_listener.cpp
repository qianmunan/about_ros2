#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h"
#include "geometry_msgs/msg/twist.hpp"

using namespace std::chrono_literals;
/**
 *
 * 需求：监听坐标变换广播的数据，并生成turtle相对于turtle2的坐标数据，进而生成控制turtle2运动的速度指令
 * 关键步骤：
 * 声明参数服务
 * 创建缓存
 * 创建监听器
 * 创建速度发布方
 * 创建一个定时器，实现坐标的变换，并且要生成速度指令并发布
 */

class Exer03TFListener: public rclcpp::Node{

    public: 
        Exer03TFListener():Node("exer03_tf_listener_node_cpp"){
            RCLCPP_INFO(this->get_logger(), "创建了listener");
            // 声明参数服务
            this->declare_parameter("father_frame","turtle2");
            this->declare_parameter("child_frame","turtle1");
            father_frame = this->get_parameter("father_frame").as_string();
            child_frame = this->get_parameter("child_frame").as_string();
            // 创建缓存
            buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
            // 创建监听器
            listener_ = std::make_shared<tf2_ros::TransformListener>(*buffer_);
            // 创建速度发布方
            cmd_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/"+father_frame+"/cmd_vel",10);
            // 创建定时器
            timer_ = this->create_wall_timer(1s,std::bind(&Exer03TFListener::on_timer,this));


        }
    private:
        void on_timer()
        {
            // 实现坐标变换
            try {
                auto ts = buffer_->lookupTransform(father_frame,child_frame,tf2::TimePointZero);
                // 组织并发布速度指令
                geometry_msgs::msg::Twist twist;
                // 明确要设置的字段
                // 线速度x和角速度z
                // 线速度和角速度的计算规则
                // ts中包含两个坐标系的x距离和y距离。
                // 线速度 = 系数 * 开方（x平方 + y平方）
                // 角速度 = 系数 * 反正切（y/x）
                twist.linear.x = 0.5 * sqrt(pow(ts.transform.translation.x,2) + pow(ts.transform.translation.y,2));
                twist.angular.z = 1.0 * atan2(ts.transform.translation.y, ts.transform.translation.x);

                cmd_pub_->publish(twist);
            }catch (const tf2::LookupException& e){
                RCLCPP_INFO(this->get_logger(), "异常提示：%s",e.what());
            }


            // 组织并发布速度指令
        }
        std::string father_frame;
        std::string child_frame;
        std::shared_ptr<tf2_ros::Buffer> buffer_;
        std::shared_ptr<tf2_ros::TransformListener> listener_;
        rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_pub_;
        rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Exer03TFListener>());
    rclcpp::shutdown();
    return 0;
}

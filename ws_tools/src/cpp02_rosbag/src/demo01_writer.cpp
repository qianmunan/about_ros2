#include <cstdio>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "rosbag2_cpp/writer.hpp"
class SimpleBagRecorder: public rclcpp::Node
{
    public:
        SimpleBagRecorder():Node("simple_bag_recorder_node_cpp"){
            RCLCPP_INFO(this->get_logger(), "消息录制对象创建！！");
            // 创建录制对象
            writer_ = std::make_unique<rosbag2_cpp::Writer>();
            // 设置磁盘文件: 相对于工作空间的一个路径
            writer_->open("my_bag");
            // 写数据
            //writer_->writer();
            sub_ = this->create_subscription<geometry_msgs::msg::Twist>("/turtle1/cmd_vel",10,std::bind(&SimpleBagRecorder::do_writer_msg, this, std::placeholders::_1));
        
        }

    private:
        void do_writer_msg(std::shared_ptr<rclcpp::SerializedMessage> msg){
            RCLCPP_INFO(this->get_logger(), "数据写出.....");
            writer_->write(msg,"turtle1/cmd_vel","geometry_msgs/msg/Twist",this->now());
            // 被写出的消息  话题名称   消息类型   时间戳
        }
        std::unique_ptr<rosbag2_cpp::Writer> writer_;
        rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_;

};


int main(int argc, char ** argv)
{
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<SimpleBagRecorder>());
    rclcpp::shutdown();
    return 0;
}

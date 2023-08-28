#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h"

using namespace std::chrono_literals;

/**
 * 先发布laser到base_link的坐标相对关系，在发布camera到base_link的坐标系相对关系，
 * 求解laser到camera的坐标相对关系
 * 1. 创建一个缓存对象，融合多个坐标系相对关系为一棵坐标树
 * 2. 创建一个监听器，绑定缓存对象，会将所有广播器广播的数据写入缓存
 * 3. 编写一个定时器，实现循环转换。
 */

class TFListener: public rclcpp::Node{
    public:
        TFListener():Node("tf_listener_node_cpp"){
            RCLCPP_INFO(this->get_logger(), "创建了监听方！！！");
            // 创建一个缓存对象，融合多个坐标系相对关系为一棵坐标树
            buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());
            // 创建监听器
            listener_ = std::make_shared<tf2_ros::TransformListener>(*buffer_, this);
            // 编写一个定时器，循环实现转换
            timer_ = this->create_wall_timer(1s,std::bind(&TFListener::on_timer,this));

        }
    private:
        void on_timer()
        {
            // 实现坐标系转换
            // geometry_msgs::msg::TransformStamped  返回值，新的坐标系
            // lookupTransform(const std::string &target_frame)  新坐标系的父坐标系
            // const std::string &source_frame  新坐标帧的子坐标
            // const tf2::TimerPoint &time  转换的时间点 一般设置为tf2_ros::TimePointZero 最新时刻的坐标帧
            try{
                auto ts = buffer_->lookupTransform("camera","laser",tf2::TimePointZero);
                RCLCPP_INFO(this->get_logger(),"---------转换完成的坐标帧信息--------");
                RCLCPP_INFO(this->get_logger(),
                        "新坐标帧：父坐标系:%s, 子坐标系：%s, 偏移量(%.2f,%.2f,%.2f)",
                        ts.header.frame_id.c_str(),
                        ts.child_frame_id.c_str(),
                        ts.transform.translation.x,
                        ts.transform.translation.y,
                        ts.transform.translation.z
                        );

            }catch(const tf2::LookupException& e)
            {
                RCLCPP_INFO(this->get_logger(), "异常提示：%s", e.what());
            }
        }
        std::unique_ptr<tf2_ros::Buffer> buffer_;
        std::shared_ptr<tf2_ros::TransformListener> listener_;
        rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TFListener>());
    rclcpp::shutdown();
    return 0;
}

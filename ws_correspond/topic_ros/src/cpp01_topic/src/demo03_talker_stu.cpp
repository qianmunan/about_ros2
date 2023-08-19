/*
  需求：以某个固定频率发送文本学生信息，包含学生的姓名、年龄、身高等数据。
*/

#include "rclcpp/rclcpp.hpp"
#include "base_interface_demo/msg/student.hpp"

using namespace std::chrono_literals;
using base_interface_demo::msg::Student;

// 定义节点类
class MinimalPublisher : public rclcpp::Node
{
public:
    MinimalPublisher() : Node("student_publish"), count_(0)
    {
        // 创建发布方
        publisher_ = this->create_publisher<Student>("topic_stu", 10);
        // 创建定时器
        timer_ = this->create_wall_timer(500ms, std::bind(&MinimalPublisher::timer_callback, this));
    }

private:
    void timer_callback()
    {
        // 组织并发布消息
        auto stu = Student();
        stu.name = "fwenqian";
        stu.age = count_++;
        stu.height = 1.60;
        RCLCPP_INFO(this->get_logger(),"学生信息: name=%s, age=%d, height=%.2f", stu.name.c_str(), stu.age, stu.height);
        publisher_->publish(stu);
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<Student>::SharedPtr publisher_;
    size_t count_;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<MinimalPublisher>());


    rclcpp::shutdown();

    return 0;
}
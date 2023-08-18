/*
   需求：处理请求发送的目标点，计算乌龟与目标点之间的直线距离。
   步骤：
       1.包含头文件；
       2.初始化 ROS2 客户端；
       3.定义节点类；
            3-1.创建乌龟姿态订阅方，回调函数中获取x坐标与y坐标；
            3-2.创建服务端；
            3-3.解析目标值，计算距离并反馈结果。
       4.调用spin函数，并传入节点对象指针；
       5.释放资源。
*/
// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "base_interfaces_demo/srv/distance.hpp"
#include "turtlesim/msg/pose.hpp"
#include "turtlesim/srv/spawn.hpp"

using namespace std::chrono_literals;
// 3.定义节点类；
class ExeDistanceServer: public rclcpp::Node {
public:
    ExeDistanceServer():Node("exe_distance_server"),turtle1_x(0.0),turtle1_y(0.0){
        // 3-1.创建乌龟姿态订阅方，回调函数中获取x坐标与y坐标；
        pose_sub = this->create_subscription<turtlesim::msg::Pose>("/turtle1/pose",10,std::bind(&ExeDistanceServer::poseCallBack, this, std::placeholders::_1));
        // 3-2.创建服务端；
        distance_server = this->create_service<base_interfaces_demo::srv::Distance>("distance",std::bind(&ExeDistanceServer::distanceCallBack, this, std::placeholders::_1, std::placeholders::_2));
    }
private:

    void poseCallBack(const turtlesim::msg::Pose::SharedPtr pose){
        turtle1_x = pose->x;
        turtle1_y = pose->y;
    }
    // 3-3.解析目标值，计算距离并反馈结果。
    void distanceCallBack(const base_interfaces_demo::srv::Distance_Request::SharedPtr request,
                    base_interfaces_demo::srv::Distance_Response::SharedPtr response
    ){
        // 解析目标值
        float goal_x = request->x;
        float goal_y = request->y;

        // 距离计算
        float x = goal_x - turtle1_x;
        float y = goal_y - turtle1_y;
        // 将结果设置到响应
        response->distance = std::sqrt(x * x + y * y);
        RCLCPP_INFO(this->get_logger(),"目标坐标:(%.2f,%.2f),距离:%.2f",goal_x,goal_y,response->distance);

    }
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_sub;
    rclcpp::Service<base_interfaces_demo::srv::Distance>::SharedPtr distance_server;
    float turtle1_x;
    float turtle1_y;


};

int main(int argc, char const *argv[])
{
    // 2.初始化 ROS2 客户端；
    rclcpp::init(argc,argv);
    // 4.调用spin函数，并传入节点对象指针；
    rclcpp::spin(std::make_shared<ExeDistanceServer>());
    // 5.释放资源。
    rclcpp::shutdown();
    return 0;
}

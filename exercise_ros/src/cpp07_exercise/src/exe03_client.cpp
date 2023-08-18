/*
   需求：发布目标点的坐标，接收并处理服务端反馈的结果。
   步骤：
       1.包含头文件；
       2.初始化 ROS2 客户端；
       3.定义节点类；
            3-1.创建客户端；
            3-2.连接服务；
            3-3.发送请求。
       4.调用对象服务连接、发送请求、处理响应相关函数；
       5.释放资源。
*/
// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "base_interfaces_demo/srv/distance.hpp"
#include "turtlesim/srv/spawn.hpp"

using namespace std::chrono_literals;

// 3.定义节点类；
class ExeDistanceClient: public rclcpp::Node {
public:
    ExeDistanceClient():Node("exe_distance_client"){
        // 3-1.创建客户端；
        distance_client = this->create_client<base_interfaces_demo::srv::Distance>("distance");
    }   
    // 3-2.连接服务；
    bool connect_server(){
      while (!distance_client->wait_for_service(1s))
      {
        if (!rclcpp::ok())
        {
          RCLCPP_INFO(this->get_logger(),"客户端退出！");
          return false;
        }

        RCLCPP_INFO(this->get_logger(),"服务连接中，请稍候...");
      }
      return true;
    }
    // 3-3.发送请求。
    rclcpp::Client<base_interfaces_demo::srv::Distance>::FutureAndRequestId send_distance(float x,float y,float theta){
        auto distance_request = std::make_shared<base_interfaces_demo::srv::Distance::Request>();
        distance_request->x = x;
        distance_request->y = y;
        distance_request->theta = theta;
        return distance_client->async_send_request(distance_request);
    }
private:
    rclcpp::Client<base_interfaces_demo::srv::Distance>::SharedPtr distance_client;
};
int main(int argc, char const *argv[])
{
    // 2.初始化 ROS2 客户端；
    rclcpp::init(argc,argv);
    // 4.调用对象服务连接、发送请求、处理响应相关函数；
    auto client = std::make_shared<ExeDistanceClient>();
    // 处理传入的参数
    if (argc != 5)
    {
        RCLCPP_INFO(client->get_logger(),"请传入目标的位姿参数:(x,y,theta)");
        return 1;
    }

    float x = atof(argv[1]);
    float y = atof(argv[2]);
    float theta = atof(argv[3]);
    // 服务连接
    bool flag = client->connect_server();
    if (!flag)
    {
        RCLCPP_INFO(client->get_logger(),"服务连接失败!");
        return 1;
    }
    // 发送请求
    auto distance_future = client->send_distance(x, y, theta);
    // 处理响应
    if(rclcpp::spin_until_future_complete(client,distance_future) == rclcpp::FutureReturnCode::SUCCESS){
        RCLCPP_INFO(client->get_logger(),"两只乌龟相距%.2f米。",distance_future.get()->distance);
    } else {
        RCLCPP_INFO(client->get_logger(),"获取距离服务失败!");
    }
    // 5.释放资源。
    rclcpp::shutdown();
    return 0;
}

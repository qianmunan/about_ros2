#include "rclcpp/rclcpp.hpp"
#include "turtlesim/srv/spawn.hpp"

using namespace std::chrono_literals;

/**
 *需求：编写客户端，发送请求生成一只新的乌龟
 步骤：
     1. 包含头文件
     2. 初始化 ROS2 客户端
     3. 定义节点类
        3.1 声明并获取参数
        3.2 创建客户端
        3.3 等待服务链接
        3.4 组织请求数据并发送
    4. 创建对象指针调用其功能，并处理响应
    5. 释放资源
 *
 * */
class Exer01TfSpawn: public rclcpp::Node{
    public:
        Exer01TfSpawn():Node("exer01_tf_spawn_node_cpp"){
        
            RCLCPP_INFO(this->get_logger(), "创建了第一个节点");
            this->declare_parameter("x", 3.0);
            this->declare_parameter("y", 3.0);
            this->declare_parameter("theta",0.0);
            this->declare_parameter("turtle_name","turtle2");
            x = this->get_parameter("x").as_double();
            y = this->get_parameter("y").as_double();
            theta = this->get_parameter("theta").as_double();
            turtle_name = this->get_parameter("turtle_name").as_string();

            // 创建服务客户端
            spawn_client_ = this->create_client<turtlesim::srv::Spawn>("/spawn");


        }
        // 链接服务端
        bool connect_server(){
            while(!spawn_client_->wait_for_service(1s)){
                if (!rclcpp::ok()){
                    RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"强制推出了");
                    return false;
                }
                RCLCPP_INFO(this->get_logger(), "服务链接中");
            }
            return true;
        }

        // 组织并发送数据
        rclcpp::Client<turtlesim::srv::Spawn>::FutureAndRequestId request(){
            auto req = std::make_shared<turtlesim::srv::Spawn::Request>();
            req->x = x;
            req->y = y;
            req->theta = theta;
            req->name = turtle_name;
            /**
             * rclcpp::Client<turtlesim::srv::Spawn>::FutureAndRequestId
             * async_send_request(std::shared_ptr<turtlesim::srv::Spawn_Request> request)
             */
            return spawn_client_->async_send_request(req);
        }
    private:
        double_t x,y,theta;
        std::string turtle_name;
        rclcpp::Client<turtlesim::srv::Spawn>::SharedPtr spawn_client_;

};

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    //rclcpp::spin(std::make_shared<Exer01TfSpawn>());
    auto client_ = std::make_shared<Exer01TfSpawn>();
    bool flag = client_->connect_server();
    if (!flag){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "服务端链接失败");
    }
    // 发送请求
    auto response = client_->request();
    // 处理响应
    if (rclcpp::spin_until_future_complete(client_,response) == rclcpp::FutureReturnCode::SUCCESS){
        RCLCPP_INFO(client_->get_logger(), "响应成功！！！");
        // 如果生成新乌龟时候，新乌龟重名了，那么也会响应成功，其实并没有生成新乌龟，且响应的结果中乌龟的名称为空串。
        // 正常应该是请求的乌龟名称
        std::string name = response.get()->name;
        if (name.empty()){
            RCLCPP_INFO(client_->get_logger(), "您生成的乌龟因重名导致失败");
        } else {
            RCLCPP_INFO(client_->get_logger(), "生成的乌龟成功");
        }
    } else {
        RCLCPP_INFO(client_->get_logger(),"响应失败！！");
    }
    // 释放资源
    rclcpp::shutdown();
    return 0;
}

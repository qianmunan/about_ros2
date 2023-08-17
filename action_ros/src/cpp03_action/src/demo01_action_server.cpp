/*  
  需求：编写动作服务端实习，可以提取客户端请求提交的整型数据，并累加从1到该数据之间的所有整数以求和，
       每累加一次都计算当前运算进度并连续反馈回客户端，最后，在将求和结果返回给客户端。
  步骤：
    1.包含头文件；
    2.初始化 ROS2 客户端；
    3.定义节点类；
      3-1.创建动作服务端；
      3-2.处理请求数据；
      3-3.处理取消任务请求；
      3-4.生成连续反馈。
    4.调用spin函数，并传入节点对象指针；
    5.释放资源。

*/
// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "base_interfaces_demo/action/progress.hpp"

using namespace std::placeholders;
using base_interfaces_demo::action::Progress;
using GoalHandleProgress = rclcpp_action::ServerGoalHandle<Progress>;

// 3.定义节点类；
class MinimalActionServer : public rclcpp::Node
{
public:

  explicit MinimalActionServer(const rclcpp::NodeOptions & options = rclcpp::NodeOptions())
  : Node("minimal_action_server", options)
  {
    // 3-1.创建动作服务端；
    this->action_server_ = rclcpp_action::create_server<Progress>(
      this,
      "get_sum",
      std::bind(&MinimalActionServer::handle_goal, this, _1, _2),
      std::bind(&MinimalActionServer::handle_cancel, this, _1),
      std::bind(&MinimalActionServer::handle_accepted, this, _1));
    RCLCPP_INFO(this->get_logger(),"动作服务端创建，等待请求...");
  }

private:
  rclcpp_action::Server<Progress>::SharedPtr action_server_;

  // 3-2.处理请求数据；
  rclcpp_action::GoalResponse handle_goal(const rclcpp_action::GoalUUID & uuid,std::shared_ptr<const Progress::Goal> goal)
  {
    (void)uuid;
    RCLCPP_INFO(this->get_logger(), "接收到动作客户端请求，请求数字为 %ld", goal->num);
    if (goal->num < 1) {
      return rclcpp_action::GoalResponse::REJECT;
    }
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
  }

  // 3-3.处理取消任务请求；
  rclcpp_action::CancelResponse handle_cancel(
    const std::shared_ptr<GoalHandleProgress> goal_handle)
  {
    (void)goal_handle;
    RCLCPP_INFO(this->get_logger(), "接收到任务取消请求");
    return rclcpp_action::CancelResponse::ACCEPT;
  }

  void execute(const std::shared_ptr<GoalHandleProgress> goal_handle)
  {
    RCLCPP_INFO(this->get_logger(), "开始执行任务");
    rclcpp::Rate loop_rate(10.0);
    const auto goal = goal_handle->get_goal();
    auto feedback = std::make_shared<Progress::Feedback>();
    auto result = std::make_shared<Progress::Result>();
    int64_t sum= 0;
    for (int i = 1; (i <= goal->num) && rclcpp::ok(); i++) {
      sum += i;
      // Check if there is a cancel request
      if (goal_handle->is_canceling()) {
        result->sum = sum;
        goal_handle->canceled(result);
        RCLCPP_INFO(this->get_logger(), "任务取消");
        return;
      }
      feedback->progress = (double_t)i / goal->num;
      goal_handle->publish_feedback(feedback);
      RCLCPP_INFO(this->get_logger(), "连续反馈中，进度：%.2f", feedback->progress);

      loop_rate.sleep();
    }

    if (rclcpp::ok()) {
      result->sum = sum;
      goal_handle->succeed(result);
      RCLCPP_INFO(this->get_logger(), "任务完成！");
    }
  }

  // 3-4.生成连续反馈。
  void handle_accepted(const std::shared_ptr<GoalHandleProgress> goal_handle)
  {
    std::thread{std::bind(&MinimalActionServer::execute, this, _1), goal_handle}.detach();
  }
}; 

int main(int argc, char ** argv)
{
  // 2.初始化 ROS2 客户端；
  rclcpp::init(argc, argv);
  // 4.调用spin函数，并传入节点对象指针；
  auto action_server = std::make_shared<MinimalActionServer>();
  rclcpp::spin(action_server);
  // 5.释放资源。
  rclcpp::shutdown();
  return 0;
}

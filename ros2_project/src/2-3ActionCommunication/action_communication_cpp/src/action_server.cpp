#include <memory>
#include <thread>
#include <chrono>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"

#include "action_interfaces/action/fibonacci.hpp"

using Fibonacci = action_interfaces::action::Fibonacci;
using GoalHandleFibonacci = rclcpp_action::ServerGoalHandle<Fibonacci>;

class FibonacciActionServer : public rclcpp::Node
{
public:
  FibonacciActionServer()
  : Node("fibonacci_action_server")
  {
    // 创建一个 ActionServer
    // 这一行代码内部会自动创建：
    // 1. 目标传递服务（Send Goal）
    // 2. 结果传递服务（Get Result）
    // 3. 取消执行服务（Cancel Goal）
    // 4. 反馈话题（Feedback Topic）
    // 5. 状态话题（Status Topic）
    this->action_server_ =
      rclcpp_action::create_server<Fibonacci>(
        this,
        "fibonacci",
        std::bind(&FibonacciActionServer::goal_callback, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&FibonacciActionServer::cancel_callback, this, std::placeholders::_1),
        std::bind(&FibonacciActionServer::execute_callback, this, std::placeholders::_1)
      );
  }

private:
  rclcpp_action::Server<Fibonacci>::SharedPtr action_server_;

  // 目标传递服务 中 服务端的回调函数
  rclcpp_action::GoalResponse goal_callback(
    const rclcpp_action::GoalUUID &,
    std::shared_ptr<const Fibonacci::Goal> goal)
  {
    RCLCPP_INFO(
      this->get_logger(),
      "收到新的目标请求，order = %d",
      goal->order
    );

    // 这里直接无条件接受
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
  }

  // 取消执行服务 中 服务端的回调函数
  rclcpp_action::CancelResponse cancel_callback(
    const std::shared_ptr<GoalHandleFibonacci> goal_handle)
  {
    RCLCPP_INFO(this->get_logger(), "收到取消目标的请求");
    return rclcpp_action::CancelResponse::ACCEPT;
  }

  // 执行任务的回调函数
  // 这里面包含了 ：
  // - Action执行 结果传递服务 服务端回调
  // - Action执行 反馈话题 发布者发布
  void execute_callback(const std::shared_ptr<GoalHandleFibonacci> goal_handle)
  {
    RCLCPP_INFO(this->get_logger(), "开始执行目标任务");

    auto feedback = std::make_shared<Fibonacci::Feedback>();
    auto result = std::make_shared<Fibonacci::Result>();

    std::vector<int> sequence;
    sequence.push_back(0);
    sequence.push_back(1);

    // 根据 Goal 中的参数决定计算长度
    for (int i = 2; i < goal_handle->get_goal()->order; ++i) {
      // 检查是否有取消请求
      if (goal_handle->is_canceling()) {
        goal_handle->canceled(result);
        RCLCPP_INFO(this->get_logger(), "目标任务已被取消");
        return;
      }

      // 任务没有被取消
      sequence.push_back(sequence[i - 1] + sequence[i - 2]);

      // 反馈话题（Feedback Topic）
      // 将当前计算进度作为反馈发送给客户端
      feedback->partial_sequence = sequence;
      goal_handle->publish_feedback(feedback);

      std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // 结果传递服务（Get Result）
    result->sequence = sequence;
    goal_handle->succeed(result);
    RCLCPP_INFO(this->get_logger(), "目标任务执行完成，返回最终结果");
  }
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<FibonacciActionServer>());
  rclcpp::shutdown();
  return 0;
}
